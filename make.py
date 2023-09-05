#! /usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Make tool

Usage:
  make.py run <task-name>
  make.py run <task-name> [options]
  make.py [options]
  make.py -h | --help

Options:
  -h --help                         Show this screen
  -d --debug                        Enable debug mode
  --version                         Show version
  --platform PLATFORM               Set qt platform to download
  --arch ARCH                       Set qt arch to download

Examples:
  python make.py -h

Tasks:
  - format
  - clean

  - build-qt
  - build-macos
  - build-win-x64
  - build-win-arm64
"""

from docopt import docopt

import modules.common as common
import modules.functions as f
import modules.config as c
import modules.macos as macos
import modules.windows as windows


def main(options):
    # show all params for debug
    if ("--debug" in options and options["--debug"]) or (
        "-d" in options and options["-d"]
    ):
        c.make_debug = True

    if c.make_debug:
        f.debug("You have executed with options:")
        f.message(str(options))
        f.message("")

    # bind options
    if "<task-name>" in options:
        make_task = options["<task-name>"]

    # validate data
    f.debug("Validating data...")

    # validate task
    if not make_task:
        f.error("Task is invalid")

    # format
    if make_task == "format":
        common.run_task_format()

    # clean
    elif make_task == "clean":
        common.run_task_clean()

    # build qt
    elif make_task == "build-qt":
        common.run_task_build_qt(options)

    #######################
    # macOS
    #######################

    # build macos
    elif make_task == "build-macos":
        macos.run_task_build()

    #######################
    # windows
    #######################

    # build windows (x64)
    elif make_task == "build-win-x64":
        windows.run_task_build(c.targets_win_x64)

    # build windows (arm64)
    elif make_task == "build-win-arm64":
        windows.run_task_build(c.targets_win_arm64)

    #######################
    # Invalid
    #######################

    # invalid
    else:
        f.error("Task is invalid")

    f.message("")
    f.debug("FINISHED!")


if __name__ == "__main__":
    # main CLI entrypoint
    args = docopt(__doc__, version="2.0.0")
    main(args)
