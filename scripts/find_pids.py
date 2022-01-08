#!/usr/bin/env python

from argparse import ArgumentParser
from monitor_lib import find_ancestor
import psutil
import sys


def main():
    arg_parser = ArgumentParser(description='find PIDs to monitor')
    arg_parser.add_argument('--process_name', required=True,
                            help='command to monitor')
    arg_parser.add_argument('--ancestor', action='store_true',
                            help='find most remote ancestor')
    arg_parser.add_argument('--user', help='user ID of processes')
    options = arg_parser.parse_args()
    pids = []
    for process in psutil.process_iter():
        if options.process_name == process.name():
            pids.append(process.pid)
    if options.ancestor:
        pid = pids[0] if len(pids) > 0 else None
        pids = [find_ancestor(pid, options.user).pid]
    print(':'.join(map(str, pids)))


if __name__ == '__main__':
    status = main()
    sys.exit(status)
