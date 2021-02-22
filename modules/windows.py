import glob
import os
import tarfile
from subprocess import check_call

import modules.config as c
import modules.functions as f

target_config = None


def run_task_build(config):
    f.debug("Building libraries...")

    target_config = config

    current_dir = os.getcwd()
    code_dir = os.path.join(current_dir, "code")
    res_dir = os.path.join(code_dir, "resources")

    # general
    project_file = os.path.join(current_dir, "PRProjector.pro")

    # targets
    for target in target_config:
        # qt
        f.debug(
            'Building Qt to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append(f.get_executable())
        command.append("make.py")
        command.append("run")
        command.append("build-qt")
        command.append("--platform")
        command.append('"{0}"'.format(target["qt_platform"]))

        command = " ".join(command)

        check_call(command, shell=True)

        # qt tools
        qt_dir = os.path.join(
            current_dir, "build", "qt", c.qt_version, target["qt_dir"]
        )
        qt_bin_dir = os.path.join(qt_dir, "bin")
        qmake_file = os.path.join(qt_bin_dir, "qmake")

        # generate files
        build_dir = os.path.join(
            current_dir,
            "build",
            target["name"],
            "{0}-{1}-{2}".format(target["name"], target["cpu"], target["mode"]),
        )

        f.remove_dir(build_dir)
        f.create_dir(build_dir)

        f.debug(
            'Generating files to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        # project
        command = []
        command.append(qmake_file)
        command.append("-project")
        command.append(project_file)

        command = " ".join(command)

        check_call(command, cwd=build_dir, shell=True)

        # makefile
        command = []
        command.append(qmake_file)
        command.append("-makefile")
        command.append(project_file)

        command = " ".join(command)

        check_call(command, cwd=build_dir, shell=True)

        # compile
        f.debug(
            'Compiling to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append("nmake")

        command = " ".join(command)

        check_call(command, cwd=build_dir, shell=True)

        # copy all frameworks
        out_dir = os.path.join(build_dir, target["mode"], "out")
        f.remove_dir(out_dir)
        f.create_dir(out_dir)

        windeployqt_file = os.path.join(qt_bin_dir, "windeployqt")

        f.debug(
            'Copying Qt frameworks to application (.exe) to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append(windeployqt_file)
        command.append(os.path.join(build_dir, target["mode"], "PRProjector.exe"))
        command.append("--dir")
        command.append(out_dir)
        command.append("--{0}".format(target["mode"]))
        command.append("--force")
        command.append("--compiler-runtime")
        command.append("--qmldir")
        command.append(os.path.join("code", "resources", "qml"))

        command = " ".join(command)

        check_call(command, shell=True)

        # copy executable
        f.debug(
            'Copying application (.exe) to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        f.copy_file(
            os.path.join(build_dir, target["mode"], "PRProjector.exe"),
            os.path.join(out_dir, "PRProjector.exe"),
        )

        # distribute package
        f.debug(
            'Distributing package (.zip) to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        pkg_dir = os.path.join(current_dir, "build", target["name"], "pkg")
        f.create_dir(pkg_dir)

        f.make_zipfile(
            os.path.join(
                pkg_dir, "PRProjector-{0}-{1}.zip".format(target["cpu"], target["mode"])
            ),
            out_dir,
        )
