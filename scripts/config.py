import pathlib
import platform
import subprocess
import shlex
from pathlib import Path

# Paths
DIR_ROOT = Path(__file__).parents[1]
DIR_BUILD_VENDOR = DIR_ROOT / "build-vendor"

# CMake
# None Debug Release RelWithDebInfo Profile
BUILD_TYPE = "Debug"

# C
CC = "clang"
C_STANDARD = "99"  # was 17 or 11

# C++
CXX = "clang++"
CXX_STANDARD = "17"

# ================================================================


def is_windows():
    return platform.system() == "Windows"


def is_macos():
    return platform.system() == "Darwin"


def is_linux():
    return platform.system() == "Linux"


def is_command_exists(command):
    pass


def exec(command, cwd=DIR_ROOT, shell=False):
    try:
        print("[Command]", command)
        result = subprocess.run(
            shlex.split(command),
            cwd=cwd,
            shell=shell,
        )

        print()
        return result
    except Exception as e:
        print("Error: %s - %s." % (e.filename, e.strerror))


def log(message):
    print("[LOG]", message)


def normalize_local_path(path):
    return (
        str(path)
        .replace(str(DIR_ROOT) + str(pathlib.os.sep), "")
        .replace(str(pathlib.os.sep), "/")
    )
