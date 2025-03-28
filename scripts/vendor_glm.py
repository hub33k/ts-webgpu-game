#!/usr/bin/env python3

"""
https://github.com/g-truc/glm
"""

import sys
from pathlib import Path

from config import exec, is_macos, is_windows, DIR_BUILD_VENDOR

BUILD_TYPE = "Debug"
CC = "clang"
C_STANDARD = "99"
CXX = "clang++"
CXX_STANDARD = "17"

DIR_SRC = Path(DIR_BUILD_VENDOR / "glm")
DIR_CODE = Path(DIR_SRC / "code")
DIR_BUILD = Path(DIR_SRC, "build")
DIR_INSTALL = Path(DIR_SRC, "install")

GIT_SOURCE_URL = "https://github.com/g-truc/glm"
GIT_HASH = "2d4c4b4"  # Update ci.yml to run CI on master branch PRs


def get_code():
    if Path.exists(DIR_CODE):
        return

    exec(f'git clone {GIT_SOURCE_URL} "{DIR_CODE}"')
    exec(f"git checkout {GIT_HASH}", cwd=DIR_CODE)


def configure():
    common_cmake_options = (
        f' -S "{DIR_CODE}" -B "{DIR_BUILD}"'
        " -G Ninja"
        #
        f" -DCMAKE_C_COMPILER={CC}"
        " -DCMAKE_C_STANDARD=17"
        " -DCMAKE_C_STANDARD_REQUIRED=ON"
        f" -DCMAKE_CXX_COMPILER={CXX}"
        " -DCMAKE_CXX_STANDARD=23"
        " -DCMAKE_CXX_STANDARD_REQUIRED=ON"
        " -DCMAKE_BUILD_TYPE=Release"
        # Library specific options
        " -DGLM_BUILD_TESTS=OFF"
        " -DBUILD_SHARED_LIBS=OFF"
    )

    if is_windows():
        command = (
            "cmake"
            #
            f"{common_cmake_options}"
        )
    elif is_macos():
        command = (
            "cmake"
            #
            f"{common_cmake_options}"
        )
    else:
        sys.exit("Only Windows and MacOS are supported")
    exec(command)


def build():
    command = (
        "cmake"
        #
        f' --build "{DIR_BUILD}"'
        " --config Release"
    )
    exec(command)


def install():
    command = (
        "cmake"
        #
        f' --install "{DIR_BUILD}"'
        f' --prefix "{DIR_INSTALL}"'
    )
    exec(command)


def main():
    # Prepare dirs
    if not Path.exists(DIR_SRC):
        Path.mkdir(DIR_SRC, parents=True)
    if not Path.exists(DIR_INSTALL):
        Path.mkdir(DIR_INSTALL, parents=True)

    get_code()

    configure()
    build()
    install()


if __name__ == "__main__":
    main()
