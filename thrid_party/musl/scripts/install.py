#!/usr/bin/env python

import os
import sys
import argparse
import subprocess
from shutil import copy

def exec_command(cmd, log_path='out/build.log', **kwargs):
    process = subprocess.Popen(cmd)
    process.wait()
    ret_code = process.returncode

    if ret_code != 0:
        raise Exception("{} failed, return code is {}".format(cmd, ret_code))

def musl_copy_file(src, dest):
    dest_dir = os.path.dirname(dest)
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    copy(src, dest)

def main():
    parser = argparse.ArgumentParser(description=__doc__)

    parser.add_argument('--input',
                        required = True,
                        help = 'The header source path',
                        metavar = 'FILE')

    parser.add_argument('--output',
                        required = True,
                        help = 'The output directory',
                        metavar = 'FILE')

    parser.add_argument('--ldpath',
                        required = True,
                        help = 'The ld file path',
                        metavar = 'FILE')

    parser.add_argument('--crtplus',
                        required = True,
                        help = 'The crtplus file path',
                        metavar = 'FILE')                        

    args = parser.parse_args()

    if os.path.basename(args.input) == 'crtn.o':
        ldargs = [args.ldpath, "-r", args.input, args.crtplus, "-o", args.output]
        exec_command(ldargs)
    else:
        musl_copy_file(args.input, args.output)

if __name__ == "__main__":
    sys.exit(main())
