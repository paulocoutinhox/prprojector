import glob
import os
import tarfile
from subprocess import check_call

import modules.config as c
import modules.functions as f


def run_task_build():
    f.debug("Building libraries...")

    current_dir = os.getcwd()
    code_dir = os.path.join(current_dir, "code")
    res_dir = os.path.join(code_dir, "resources")

    # general
    project_file = os.path.join(current_dir, "PRProjector.pro")

    # targets
    for target in c.targets_macos:
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
        command.append("make")

        command = " ".join(command)

        check_call(command, cwd=build_dir, shell=True)

        # plist
        f.debug(
            'Copying plist to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        f.copy_file(
            os.path.join(res_dir, "macos", "Info.plist"),
            os.path.join(build_dir, "PRProjector.app", "Contents", "Info.plist"),
        )

        # dSYM
        f.debug(
            'Generating dSYM to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append("dsymutil")
        command.append(
            os.path.join(
                build_dir, "PRProjector.app", "Contents", "MacOS", "PRProjector"
            )
        )
        command.append("-o")
        command.append(os.path.join(build_dir, "PRProjector.app.dSYM"))

        command = " ".join(command)

        check_call(command, shell=True)

        # copy all frameworks
        macdeployqt_file = os.path.join(qt_bin_dir, "macdeployqt")

        f.debug(
            'Copying Qt frameworks to application (.app) to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append(macdeployqt_file)
        command.append(os.path.join(build_dir, "PRProjector.app"))
        command.append("-qmldir=code/resources/qml")

        command = " ".join(command)

        check_call(command, shell=True)

        # codesign frameworks
        f.debug(
            'Codesign frameworks to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        frameworks_dir = os.path.join(
            build_dir, "PRProjector.app", "Contents", "Frameworks"
        )
        frameworks = f.list_subdirs(frameworks_dir)

        for framework in frameworks:
            framework_dir = os.path.join(frameworks_dir, framework)

            f.debug(
                'Codesign framework {0} to arch "{1}" and mode "{2}"...'.format(
                    framework, target["cpu"], target["mode"]
                )
            )

            command = []
            command.append("codesign")
            command.append("-f")
            command.append("-s")
            command.append('"{0}"'.format(c.macos_dist_certificate_name))
            command.append(framework_dir)

            command = " ".join(command)

            check_call(command, shell=True)

        # codesign plugins
        f.debug(
            'Codesign plugins to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        plugins_dir = os.path.join(build_dir, "PRProjector.app", "Contents", "PlugIns")
        plugins = f.find_files_with_path(plugins_dir + "/**/*.dylib")

        for plugin in plugins:
            plugin_file = os.path.basename(plugin)

            f.debug(
                'Codesign plugin {0} to arch "{1}" and mode "{2}"...'.format(
                    plugin_file, target["cpu"], target["mode"]
                )
            )

            command = []
            command.append("codesign")
            command.append("-f")
            command.append("-s")
            command.append('"{0}"'.format(c.macos_dist_certificate_name))
            command.append(plugin)

            command = " ".join(command)

            check_call(command, shell=True)

        # codesign application
        f.debug(
            'Codesign application (.app) to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append("codesign")
        command.append("-f")
        command.append("-s")
        command.append('"{0}"'.format(c.macos_dist_certificate_name))
        command.append("-v")
        command.append("--entitlements")
        command.append("code/resources/macos/Entitlements.plist")
        command.append(os.path.join(build_dir, "PRProjector.app"))

        command = " ".join(command)

        check_call(command, shell=True)

        # building package
        pkg_dir = os.path.join("build", target["name"], "pkg")

        f.create_dir(pkg_dir)

        f.debug(
            'Building package (.pkg) to arch "{0}" and mode "{1}"...'.format(
                target["cpu"], target["mode"]
            )
        )

        command = []
        command.append("productbuild")
        command.append("--component")
        command.append(os.path.join(build_dir, "PRProjector.app"))
        command.append("/Applications")
        command.append("--sign")
        command.append('"{0}"'.format(c.macos_installer_certificate_name))
        command.append(
            os.path.join(
                pkg_dir, "PRProjector-{0}-{1}.pkg".format(target["cpu"], target["mode"])
            )
        )

        command = " ".join(command)

        check_call(command, shell=True)
