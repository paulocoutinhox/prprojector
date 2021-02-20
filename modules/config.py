# general
make_debug = False
make_task = ""

# qt
qt_version = "5.15.2"

# macos
macos_dist_certificate_name = "3rd Party Mac Developer Application: Paulo Coutinho (99AHAA343Q)"
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
