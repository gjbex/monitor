#!/usr/bin/env python

from argparse import ArgumentParser
from monitor_lib import find_pids
import sys


def main():
    arg_parser = ArgumentParser(description='find PIDs to monitor')
    arg_parser.add_argument('--process_name', required=True,
                            help='command to monitor')
    arg_parser.add_argument('--ancestor', action='store_true',
                            help='find most remote ancestor')
    arg_parser.add_argument('--user', help='user ID of processes')
    options = arg_parser.parse_args()
    print(':'.join(map(str, find_pids(options.process_name, options.user, options.ancestor))))


if __name__ == '__main__':
    status = main()
    sys.exit(status)
