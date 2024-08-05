/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <benchmark/benchmark.h>
#include <err.h>
#include <getopt.h>
#include <cinttypes>
#include <cmath>
#include <sys/resource.h>
#include <sys/stat.h>

#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <regex>
#include <vector>

#include "util.h"

extern "C" {
#include "cJSON.h"
}

static const std::vector<int> commonArgs {
    8,
    16,
    32,
    64,
    512,
    1 * K,
    8 * K,
    16 * K,
    32 * K,
    64 * K,
    128 * K,
};

std::map<std::string, std::pair<BenchmarkFunc, std::string>> g_allBenchmarks;
std::mutex g_benchmarkLock;
std::map<std::string, std::pair<BenchmarkFunc, ApplyBenchmarkFunc>> g_applyBenchmarks;

using args_vector = std::vector<std::vector<int64_t>>;

static struct option g_benchmarkLongOptions[] = {
    {"musl_cpu", required_argument, nullptr, 'c'},
    {"musl_iterations", required_argument, nullptr, 'i'},
    {"musl_json", required_argument, nullptr, 'j'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0},
    };

void PrintUsageAndExit()
{
    printf("Usage:\n");
    printf("musl_benchmarks [--musl_cpu=<cpu_to_isolate>]\n");
    printf("                [--musl_iterations=<num_iter>]\n");
    printf("                [--musl_json=<path_to_json>]\n");
    printf("                [<original benchmark flags>]\n");
    printf("benchmark flags:\n");

    int argc = 2;
    char argv0[] = "musl_benchmark";
    char argv1[] = "--help";
    char *argv[3]{argv0, argv1, nullptr};
    benchmark::Initialize(&argc, argv);
    exit(1);
}

void ShiftOptions(int argc, char **argv, std::vector<char *> *argvAfterShift)
{
    (*argvAfterShift)[0] = argv[0];
    for (int i = 1; i < argc; ++i) {
        char *optarg = argv[i];
        size_t index = 0;
        // Find if musl defined this arg.
        while (g_benchmarkLongOptions[index].name && strncmp(g_benchmarkLongOptions[index].name, optarg + 2,
            strlen(g_benchmarkLongOptions[index].name))) {
            ++index;
        }
        // Not defined.
        if (!g_benchmarkLongOptions[index].name) {
            argvAfterShift->push_back(optarg);
        } else if ((g_benchmarkLongOptions[index].has_arg == required_argument) && !strchr(optarg, '=')) {
            i++;
        }
    }
    argvAfterShift->push_back(nullptr);
}

bench_opts_t ParseOptions(int argc, char **argv)
{
    bench_opts_t opts;
    int opt;

    while ((opt = getopt_long(argc, argv, "c:i:j:h", g_benchmarkLongOptions, nullptr)) != -1) {
        int decimal = 10;
        switch (opt) {
            case 'c':
                if (*optarg) {
                    char *errorCheck;
                    opts.cpuNum = strtol(optarg, &errorCheck, decimal);
                    if (*errorCheck) {
                        errx(1, "ERROR: Args %s is not a valid integer.", optarg);
                    }
                } else {
                    printf("ERROR: no argument specified for musl_cpu.\n");
                    PrintUsageAndExit();
                }
                break;
            case 'i':
                if (*optarg) {
                    char *errorCheck;
                    opts.iterNum = strtol(optarg, &errorCheck, decimal);
                    if (*errorCheck != '\0' or opts.iterNum < 0) {
                        errx(1, "ERROR: Args %s is not a valid number of iterations.", optarg);
                    }
                } else {
                    printf("ERROR: no argument specified for musl_iterations.\n");
                    PrintUsageAndExit();
                }
                break;
            case 'j':
                if (*optarg) {
                    opts.jsonPath = optarg;
                } else {
                    printf("ERROR: no argument specified for musl_json\n");
                    PrintUsageAndExit();
                }
                break;
            case 'h':
                PrintUsageAndExit();
                break;
            case '?':
                break;
            default:
                exit(1);
        }
    }
    return opts;
}

void LockAndRun(benchmark::State &state, BenchmarkFunc func, int cpuNum)
{
#if not defined __APPLE__
    if (cpuNum >= 0) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(cpuNum, &cpuset);

        if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0) {
            printf("lock CPU failed, ERROR:%s\n", strerror(errno));
        }
    }
#endif

    reinterpret_cast<void (*)(benchmark::State &)>(func)(state);
}

args_vector *ResolveArgs(args_vector *argsVector, std::string args,
    std::map<std::string, args_vector> &presetArgs)
{
    // Get it from preset args.
    if (presetArgs.count(args)) {
        return &presetArgs[args];
    }

    // Convert string to int.
    argsVector->push_back(std::vector<int64_t>());
    std::stringstream sstream(args);
    std::string argstr;
    while (sstream >> argstr) {
        char *errorCheck;
        int converted = static_cast<int>(strtol(argstr.c_str(), &errorCheck, 10));
        if (*errorCheck) {
            errx(1, "ERROR: Args str %s contains an invalid macro or int.", args.c_str());
        }
        (*argsVector)[0].push_back(converted);
    }
    return argsVector;
}

bool MatchFuncNameInJson(const std::string& str, const std::string& pattern)
{
    size_t jsonwildcard = pattern.find("*");
    if (jsonwildcard == std::string::npos) {
        return str == pattern;
    }
    std::string prefix = pattern.substr(0, jsonwildcard);
    std::string jsonFuncName = prefix + ".*";
    std::regex re(jsonFuncName, std::regex::icase);
    return std::regex_match(str, re);
}

static args_vector GetArgs(const std::vector<int> &sizes)
{
    args_vector args;
    for (int size : sizes) {
        args.push_back({size});
    }
    return args;
}

static args_vector GetArgs(const std::vector<int> &sizes, int value)
{
    args_vector args;
    for (int size : sizes) {
        args.push_back({size, value});
    }
    return args;
}

static args_vector GetArgs(const std::vector<int> &sizes, int value1, int value2)
{
    args_vector args;
    for (int size : sizes) {
        args.push_back({size, value1, value2});
    }
    return args;
}

std::map<std::string, args_vector> GetPresetArgs()
{
    std::map<std::string, args_vector> presetArgs {
        {"COMMON_ARGS", GetArgs(commonArgs)},
        {"ALIGNED_ONEBUF", GetArgs(commonArgs, 0)},
        {"ALIGNED_TWOBUF", GetArgs(commonArgs, 0, 0)},
        {"BENCHMARK_5", args_vector{{0}, {1}, {2}, {3}, {4}}},
        {"BENCHMARK_8", args_vector{{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}}},
        {"BENCHMARK_22", args_vector{{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13},
                                     {14}, {15}, {16}, {17}, {18}, {19}, {20}, {21}}},
    };

    return presetArgs;
}

void RegisterSingleBenchmark(bench_opts_t optsFromJson, bench_opts_t optsFromCommandLine,
    const std::string &funcName, args_vector *runArgs)
{
    bool isApplyUseCase = false;
    if (g_allBenchmarks.find(funcName) != g_allBenchmarks.end()) {
        isApplyUseCase = false;
    } else if (g_applyBenchmarks.find(funcName) != g_applyBenchmarks.end()) {
        isApplyUseCase = true;
    } else {
        errx(1, "ERROR: No benchmark for function %s", funcName.c_str());
    }

    long iterNum = optsFromCommandLine.iterNum ? optsFromCommandLine.iterNum : optsFromJson.iterNum;
    int cpuNum = -1;
    if (optsFromCommandLine.cpuNum >= 0) {
        cpuNum = optsFromCommandLine.cpuNum;
    } else if (optsFromJson.cpuNum >= 0) {
        cpuNum = optsFromJson.cpuNum;
    }

    if (isApplyUseCase) {
        BenchmarkFunc func = g_applyBenchmarks.at(funcName).first;
        auto registration = benchmark::RegisterBenchmark(funcName.c_str(), LockAndRun, func,
            cpuNum)->Apply(g_applyBenchmarks.at(funcName).second);
        if (iterNum > 0) {
            registration->Iterations(iterNum);
        }
    } else {
        BenchmarkFunc func = g_allBenchmarks.at(funcName).first;
        for (const std::vector<int64_t> &args : (*runArgs)) {
            // It will call LockAndRun(func, opts.cpuNum).
            auto registration = benchmark::RegisterBenchmark(funcName.c_str(), LockAndRun, func, cpuNum)->Args(args);
            if (iterNum > 0) {
                registration->Iterations(iterNum);
            }
        }
    }
}

void RegisterAllBenchmarks(const bench_opts_t &opts, std::map<std::string, args_vector> &presetArgs)
{
    for (auto &entry : g_allBenchmarks) {
        auto &funcInfo = entry.second;
        args_vector argVector;
        args_vector *runArgs = ResolveArgs(&argVector, funcInfo.second, presetArgs);
        RegisterSingleBenchmark(bench_opts_t(), opts, entry.first, runArgs);
    }

    for (auto &entry : g_applyBenchmarks) {
        args_vector *runArgs = nullptr;
        RegisterSingleBenchmark(bench_opts_t(), opts, entry.first, runArgs);
    }
}

std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int RegisterJsonBenchmarks(const bench_opts_t &opts, std::map<std::string, args_vector> &presetArgs)
{
    char *file = nullptr;
    cJSON *json = nullptr;
    // Read JSON string from file
    file = ReadJsonFile(opts.jsonPath.c_str());
    if (file == nullptr) {
        printf("fail to read file or no data read.\n");
        return JOSN_ERROR_FILE_READ_FAILED;
    }

    // Load JSON data
    json = cJSON_Parse(file);
    if (json == nullptr) {
        printf("JSON parsing failed, incorrect JSON format.\n");
        return JOSN_ERROR_JSON_FORMAT;
    }

    // Parsing Fields
    cJSON *item = cJSON_GetObjectItem(json, "InterfaceUsecases");
    if (item) {
        int arraySize = cJSON_GetArraySize(item); // Get the size of the array

        // Parsing each member in an array and register the functions.
        for (int i = 0; i < arraySize; i++) {
            cJSON *arrayItem = cJSON_GetArrayItem(item, i); // Extract array subscript object
            if (arrayItem == nullptr) {
                continue;
            }

            // Parsing data
            cJSON *obj = cJSON_GetObjectItem(arrayItem, "name");
            std::string fnName;
            if (obj != nullptr) {
                fnName = std::string(obj->valuestring);
            } else {
                printf("missing name element or error parsing name text\n");
            }

            obj = cJSON_GetObjectItem(arrayItem, "args");
            std::string jsonArgs;
            args_vector argVector;
            args_vector *runArgs = nullptr;
            if (obj != nullptr) {
                jsonArgs = std::string(obj->valuestring);
                runArgs = ResolveArgs(&argVector, Trim(jsonArgs), presetArgs);
            } else {
                runArgs = ResolveArgs(&argVector, "", presetArgs);
            }

            bench_opts_t jsonOpts{};
            obj = cJSON_GetObjectItem(arrayItem, "iterations");
            if (obj != nullptr) {
                jsonOpts.iterNum = obj->valueint;
            }

            obj = cJSON_GetObjectItem(arrayItem, "cpu");
            if (obj != nullptr) {
                jsonOpts.cpuNum = obj->valueint;
            }

            std::vector<std::string> matchedFuncNames;
            for (const auto& muslbenchmark : g_allBenchmarks) {
                if (MatchFuncNameInJson(muslbenchmark.first, fnName)) {
                    matchedFuncNames.push_back(muslbenchmark.first);
                }
            }

            for (const auto& useapply : g_applyBenchmarks) {
                if (MatchFuncNameInJson(useapply.first, fnName)) {
                    matchedFuncNames.push_back(useapply.first);
                }
            }

            if (matchedFuncNames.empty()) {
                errx(1, "JSONERROR: No benchmark found for function like %s in matchedFuncNames", fnName.c_str());
            }

            for (const auto& matchedFuncName : matchedFuncNames) {
                fnName = matchedFuncName;
                RegisterSingleBenchmark(jsonOpts, opts, fnName, runArgs);
            }
        }
    }

    if (json != nullptr) {
        cJSON_Delete(json);
    }
    return JSON_SUCCESS;
}

static bool IsRegularFileExists(const std::string& file)
{
    struct stat st;
    return stat(file.c_str(), &st) != -1 && S_ISREG(st.st_mode);
}

int main(int argc, char **argv)
{
    std::map<std::string, args_vector> presetArgs = GetPresetArgs();
    bench_opts_t opts = ParseOptions(argc, argv);
    std::vector<char *> argvAfterShift(argc);
    ShiftOptions(argc, argv, &argvAfterShift);

    if (opts.jsonPath.empty()) {
        RegisterAllBenchmarks(opts, presetArgs);
    } else if (!IsRegularFileExists(opts.jsonPath)) {
        std::string file("suites" + opts.jsonPath);
        if (opts.jsonPath[0] == '/' || !IsRegularFileExists(file)) {
            printf("Cannot find json file %s: does not exist or is not a file.\n", opts.jsonPath.c_str());
            return 1;
        }
        opts.jsonPath = file;
    }

    if (!opts.jsonPath.empty()) {
        if (int err = RegisterJsonBenchmarks(opts, presetArgs)) {
            return err;
        }
    }

    if (setpriority(PRIO_PROCESS, 0, -20)) {
        perror("Set priority of process failed.\n");
    }
    int argcAfterShift = argvAfterShift.size();
    benchmark::Initialize(&argcAfterShift, argvAfterShift.data());
    benchmark::RunSpecifiedBenchmarks();
}
