#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2023 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#/

import argparse
import re


class BenchmarkDataParser:
    def __init__(self):
        self.benchmark_data_before = {}
        self.benchmark_data_after = {}
        self.all_cases = []

    @staticmethod
    def get_format(before_data, after_data, pct, fluctuation_range):
        deg_format = "\033[1;31m|{:60}|{:20}|{:20}|{:20}\033[0m|"
        opt_format = "\033[1;32m|{:60}|{:20}|{:20}|{:20}\033[0m|"
        equal_format = "\033[1;37m|{:60}|{:20}|{:20}|{:20}\033[0m|"
        if pct <= fluctuation_range:
            return equal_format
        if before_data > after_data:
            return opt_format
        else:
            return deg_format

    @staticmethod
    def print_header(args):
        header_format = "\033[1;37m|{:60}|{:20}|{:20}|{:20}|\033[0m"
        print("\033[1;34m[Notice]:\033[0m")
        print("file before optimization: " + args.before)
        print("file after optimization: " + args.after)
        print("allowed fluctuation range: " + args.range + "%")
        print("\033[1;32mgreen is better, \033[0m" +
              "\033[1;31mred is worse, \033[0m" +
              "\033[1;37mwhite is equal.\033[0m")
        print("\033[1;34m[Compare Result]:\033[0m")
        print(header_format.format("case", "before(ns)", "after(ns)", "(before - after / before)"))
        print(header_format.format("----", "----", "----", "----"))

    def read_file(self, file_path, is_before):
        bench_pattern = re.compile(r'''
            ^([\S]+)   # case name
            [^0-9]+
            ([0-9,.]+) # time
            [^0-9]+
            ([0-9|.]+) # cpu
            [^0-9]+
            ([0-9|.]+) # iters
            .*''', re.VERBOSE)

        if is_before:
            current_data = self.benchmark_data_before
        else:
            current_data = self.benchmark_data_after

        lines = open(file_path, 'r').readlines()
        for line in lines:
            if line.lower().startswith("bm_"):
                m = bench_pattern.match(line)
                if m:
                    case, time, cpu, iters = m.groups()
                    self.all_cases.append(case) if self.all_cases.count(case) == 0 else self.all_cases
                    current_data[case] = [float(time), float(cpu), float(iters)]
                else:
                    print("match error: {}".format(line))

    def out_results(self, args):
        self.print_header(args)
        for case in self.all_cases:
            before_data = self.benchmark_data_before[case][0] if case in self.benchmark_data_before.keys() else 0
            after_data = self.benchmark_data_after[case][0] if case in self.benchmark_data_after.keys() else 0
            pct = 0 if before_data == 0 else round((abs(before_data - after_data) / before_data) * 100, 2)
            sign = "+" if (before_data - after_data) > 0  else "-"
            output_format = self.get_format(before_data, after_data, pct, float(args.range))
            signed_pct = "{}{}{}".format(sign, str(pct), "%")
            print(output_format.format(str(case), str(before_data), str(after_data), signed_pct))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-b', '--before', type=str, help='file before optimization')
    parser.add_argument('-a', '--after', type=str, help='file after optimization')
    parser.add_argument('-r', '--range', type=str, help='allowed fluctuation range')

    args = parser.parse_args()

    benchmark_data_parser = BenchmarkDataParser()
    benchmark_data_parser.read_file(args.before,  1)
    benchmark_data_parser.read_file(args.after, 0)
    benchmark_data_parser.out_results(args)


if __name__ == '__main__':
    main()