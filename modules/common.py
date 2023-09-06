import os
import subprocess
from subprocess import run

import modules.config as c
import modules.functions as f


def run_task_build_qt(options):
    f.debug("Building Qt...")

    build_dir = os.path.join("build")
    f.create_dir(build_dir)

    qt_platform = options["--platform"] if "--platform" in options else None

    if not qt_platform:
        f.error("Parameter --platform was not defined")

    qt_arch = options["--arch"] if "--arch" in options else None

    if not qt_platform:
        f.error("Parameter --arch was not defined")

    qt_out = options["--out"] if "--out" in options else None

    if not qt_out:
        qt_out = os.path.join(build_dir, "qt")
        f.remove_dir(qt_out)
        f.create_dir(qt_out)

    f.debug(f"Out directory: {qt_out}")

    command = []
    command.append("aqt")
    command.append("install-qt")
    command.append("-O")
    command.append(qt_out)
    command.append(qt_platform)
    command.append(c.qt_version)
    command.append(qt_arch)
    command.append("-m")
    command.append("all")

    command = " ".join(command)

    run(command, shell=True, check=True)


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
    run(command, shell=True, check=True)

    # modules
    command = " ".join(
        [
            "black",
            "modules/",
        ]
    )
    run(command, shell=True, check=True)

    f.debug("Finished")
