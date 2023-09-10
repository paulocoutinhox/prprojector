# general
make_debug = False
make_task = ""

# qt
qt_version = "6.5.2"

# macos
macos_pkg_version = "2.0.0"
macos_identifier = "com.paulocoutinho.prprojector"

macos_dist_certificate_name = "Apple Distribution: Paulo Coutinho (99AHAA343Q)"
macos_installer_certificate_name = (
    "3rd Party Mac Developer Installer: Paulo Coutinho (99AHAA343Q)"
)

targets_macos = [
    {
        "name": "macos",
        "cpu": "x64",
        "mode": "release",
        "qt_platform": "mac desktop",
        "qt_dir": "macos",
        "qt_arch": "clang_64",
    },
]

# windows
targets_win_arm64 = [
    {
        "name": "win",
        "cpu": "arm64",
        "mode": "release",
        "qt_platform": "windows desktop",
        "qt_dir": "msvc2019_arm64",
        "qt_arch": "win64_msvc2019_amd64_arm64",
    },
]

targets_win_x64 = [
    {
        "name": "win",
        "cpu": "x64",
        "mode": "release",
        "qt_platform": "windows desktop",
        "qt_dir": "msvc2019_64",
        "qt_arch": "win64_msvc2019_64",
    },
]
