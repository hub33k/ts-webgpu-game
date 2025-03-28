#!/usr/bin/env python3

"""
https://wiki.libsdl.org/SDL3/Installation
https://wiki.libsdl.org/SDL3/README/cmake
https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md#sdl-specific-cmake-options
"""

import sys
from pathlib import Path

from config import exec, is_macos, is_windows, DIR_BUILD_VENDOR

BUILD_TYPE = "Debug"
CC = "clang"
C_STANDARD = "99"
CXX = "clang++"
CXX_STANDARD = "17"

DIR_SRC = Path(DIR_BUILD_VENDOR / "sdl")
DIR_CODE = Path(DIR_SRC / "code")
DIR_BUILD = Path(DIR_SRC, "build")
DIR_INSTALL = Path(DIR_SRC, "install")

GIT_SOURCE_URL = "https://github.com/libsdl-org/SDL.git"
GIT_HASH = (
    "aef240b"  # Fixed continuous logging if returning to desktop mode fails on X11
)


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
        # " -DCMAKE_OSX_ARCHITECTURES=x86_64;arm64"
        " -DCMAKE_BUILD_TYPE=Release"
        # Library specific options
        # " -DSDL_SHARED=OFF"
        # " -DSDL_STATIC=ON"
        # " -DSDL_FRAMEWORK=OFF"
        " -DSDL_TEST_LIBRARY=OFF"
        " -DSDL_TESTS=OFF"
        " -DSDL_INSTALL_TESTS=OFF"
    )

    if is_windows():
        command = (
            "cmake"
            #
            f"{common_cmake_options}"
            # Library specific options
            " -DSDL_SHARED=OFF"
            " -DSDL_STATIC=ON"
            " -DSDL_FRAMEWORK=OFF"
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
