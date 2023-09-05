# general
make_debug = False
make_task = ""

# qt
qt_version = "6.5.2"

# macos
macos_pkg_version = "1.0.1"
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
targets_win32 = [
    {
        "name": "win32",
        "cpu": "x86",
        "mode": "release",
        "qt_platform": "windows desktop",
        "qt_dir": "msvc2019",
        "qt_arch": "win32_msvc2019",
    },
]

targets_win64 = [
    {
        "name": "win64",
        "cpu": "x64",
        "mode": "release",
        "qt_platform": "windows desktop",
        "qt_dir": "msvc2019_64",
        "qt_arch": "win64_msvc2019_64",
    },
]
