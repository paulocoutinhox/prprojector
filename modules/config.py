# general
make_debug = False
make_task = ""

# qt
qt_version = "5.15.2"

# macos
macos_pkg_version = "1.0.0"
macos_identifier = "com.prsolucoes.prprojector"

macos_dist_certificate_name = (
    "3rd Party Mac Developer Application: Paulo Coutinho (99AHAA343Q)"
)
macos_installer_certificate_name = (
    "3rd Party Mac Developer Installer: Paulo Coutinho (99AHAA343Q)"
)

targets_macos = [
    {
        "name": "macos",
        "cpu": "x64",
        "mode": "release",
        "qt_platform": "mac desktop",
        "qt_dir": "clang_64",
    },
]

# windows
targets_win32 = [
    {
        "name": "windows",
        "cpu": "x86",
        "mode": "release",
        "qt_platform": "windows desktop win32_msvc2019",
        "qt_dir": "msvc2019",
    },
]

targets_win64 = [
    {
        "name": "windows",
        "cpu": "x64",
        "mode": "release",
        "qt_platform": "windows desktop win64_msvc2019_64",
        "qt_dir": "msvc2019_64",
    },
]
