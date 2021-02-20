import os
import subprocess
from subprocess import check_call

import modules.config as c
import modules.functions as f


def run_task_build_qt(options):
    f.debug("Building Qt...")

    qt_platform = options["--platform"] if "--platform" in options else None

    if not qt_platform:
        f.error("Parameter --platform was not defined")

    build_dir = os.path.join("build")
    f.create_dir(build_dir)

    qt_dir = os.path.join(build_dir, "qt")
    f.remove_dir(qt_dir)
    f.create_dir(qt_dir)

    command = []
    command.append("aqt")
    command.append("install")
    command.append("-O")
    command.append(qt_dir)
    command.append(c.qt_version)
    command.append(qt_platform)

    command = " ".join(command)

    check_call(command, shell=True)


def run_task_clean():
    current_dir = os.getcwd()
    code_dir = os.path.join(current_dir, "code")

    f.debug("Cleaning...")

    f.remove_dir(os.path.join(current_dir, "build"))
    f.remove_file(os.path.join(code_dir, "qrc_Resources.cpp"))
    f.remove_file(os.path.join(current_dir, "aqtinstall.log"))
    f.remove_file(os.path.join(current_dir, "PRProjector.pro.user"))

    moc_files = f.find_files_regexp(code_dir, "moc_*")

    for moc_file in moc_files:
        f.remove_file(moc_file)

    ui_files = f.find_files_regexp(code_dir, "ui_*")

    for ui_file in ui_files:
        f.remove_file(ui_file)


def run_task_format():
    # check
    try:
        subprocess.check_output(["black", "--version"])
    except OSError:
        f.error("Black is not installed, check: https://github.com/psf/black")

    # start
    f.debug("Formating...")

    # make.py
    command = " ".join(
        [
            "black",
            "make.py",
        ]
    )
    check_call(command, shell=True)

    # modules
    command = " ".join(
        [
            "black",
            "modules/",
        ]
    )
    check_call(command, shell=True)

    f.debug("Finished")
