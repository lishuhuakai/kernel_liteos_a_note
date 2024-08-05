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
import copy
import json
import os
import re
import sys
import subprocess

# the absolute path to llvm-readelf
READELF_PATH = ""
# the absolute path to hdc
HDC = ""
DEBUG = 0


class SoParser:
    def __init__(self, cpu):
        self.soname_deps = {}
        self.pulled_so = []
        self.visiting = []
        self.visited = []
        self.saved_so_path = []
        self.search_paths  = ""

    @staticmethod
    def run_cmd(cmd):
        if DEBUG:
            print("CMD: " + cmd)
        process = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            outs, errs = process.communicate(timeout=10)
        except subprocess.TimeoutExpired:
            process.kill()
            print("timeout error:", " ".join(cmd), errs)
        return outs.decode().splitlines()

    @staticmethod
    def get_soname(line):
        m = re.search("[\S\s]+\[(\S+)\]", line)
        if not m:
            return ""
        soname = m.group(1)
        return soname

    def pull_so(self, path):
        cmd = "{} file recv {} {}".format(HDC, path, self.saved_so_path)
        lines = self.run_cmd(cmd)
        for line in lines:
            if "success" in line:
                return 1
            return 0
    
    def is_path_exists(self, path):
        cmd = "{} shell ls {}".format(HDC, path)
        lines = self.run_cmd(cmd)
        for line in lines:
            if "No such file or directory" in line:
                return 0
        return 1

    def write_results(self, out_format):
        if out_format is None:
            self.write_dot()
            self.write_csv()
            self.write_json()
            self.write_txt()
        if out_format == "csv":
            self.write_csv()
        if out_format == "txt":
            self.write_txt()
        if out_format == "json":
            self.write_json()
        if out_format == "dot":
            self.write_dot()

    def write_dot(self):
        with os.fdopen(os.open("dep.dot", os.O_CREAT | os.O_WRONLY, 0o755), 'w', encoding='utf-8') as f:
            f.write("digraph deps {\n")
            dot_format = "\"{}\"->\"{}\" [label={}];\n"
            for soname, deps in self.soname_deps.items():
                index = 0
                for so in deps:
                    index += 1
                    f.write(dot_format.format(soname, so, index))
            f.write("}\n")
            f.close()
        self.run_cmd("dot -Tsvg -o dep.svg dep.dot")

    def write_json(self):
        with os.fdopen(os.open("dep.json", os.O_CREAT | os.O_WRONLY, 0o755), 'w', encoding='utf-8') as f:
            to_json = json.dumps(self.soname_deps)
            f.write(to_json)
            f.close()

    def write_txt(self):
        with os.fdopen(os.open("dep.ext", os.O_CREAT | os.O_WRONLY, 0o755), 'w', encoding='utf-8') as f:
            for soname, deps in self.soname_deps.items():
                f.write("{}:{}\n".format(soname, ",".join(deps)))
            f.close()

    def write_csv(self):
        with os.fdopen(os.open("dep.csv", os.O_CREAT | os.O_WRONLY, 0o755), 'w', encoding='utf-8') as f:
            for soname, deps in self.soname_deps.items():
                f.write("{},{}\n".format(soname, ",".join(deps)))
            f.close()

    def read_soinfo(self, so_path_local):
        lines = self.run_cmd("{} -dW {}".format(READELF_PATH, so_path_local))
        deps = []
        entry_so = ""
        for line in lines:
            if "Library soname" in line:
                entry_so = self.get_soname(line)
            if "NEEDED" in line:
                deps.append(self.get_soname(line))
            
        self.soname_deps[entry_so] = deps
        return deps

    def find_so(self, so_name):
        if so_name in self.pulled_so:
            print(so_name + " already saved in " + self.saved_so_path)
            return 1
        for path in self.search_paths:
            abs_path = os.path.join(path, so_name)
            if self.is_path_exists(abs_path):
                if self.pull_so(abs_path) == 0:
                    print("pull so failed!")
                    exit(-1)
                self.pulled_so.append(so_name)
                return 1
        return 0

    def bfs(self, so_name, skip_pull_so):
        self.visiting.append(so_name)
        index = 0
        while len(self.visiting) > 0:
            cur = self.visiting.pop(0)
            if cur in self.visited:
                continue
            index += 1
            if not skip_pull_so:
                if self.find_so(cur) == 0:
                    print("can't find {}".format(cur))
                    exit(-1)
            
            deps = self.read_soinfo(self.saved_so_path + cur)
            print("\033[1;33m index:{} so:{} deps:[{}]\033[0m \n".format(index, cur, ",".join(deps)))
            if deps:
                self.visiting.extend(deps)
            self.visited.append(cur)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=str, required=True, help='Input so name that you want to show its deps.')
    parser.add_argument('-f', '--format', type=str, help='Set output file format: csv, json, dot, txt, default is all.')
    parser.add_argument('-s', '--skip-pull-so', action='store_true', help='You can skip pull so from device if you have done it.')
    parser.add_argument('-c', '--cpu', type=str, required=True, help='Set device type: 64 or 32 system.')
    parser.add_argument('-p', '--path', type=str, default="paths.json", help='Set so search paths in your device.')
    args = parser.parse_args()

    so_parser = SoParser(args.cpu)
    with os.fdopen(os.open(args.path, os.O_RDONLY, 0o755), 'r', encoding='utf-8') as f:
        so_parser.search_paths = json.load(f)[args.cpu]
    so_parser.saved_so_path = os.path.join(os.getcwd(), "saved/")
    if not args.skip_pull_so:
        so_parser.run_cmd("rm -rf {} ".format(so_parser.saved_so_path))
        so_parser.run_cmd("mkdir {}".format(so_parser.saved_so_path))
    so_parser.bfs(args.input, args.skip_pull_so)
    so_parser.write_results(args.format)


if __name__ == "__main__":
    sys.exit(main())
