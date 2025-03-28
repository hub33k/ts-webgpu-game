#!/usr/bin/env python3

"""
https://dawn.googlesource.com/dawn/+/HEAD/docs/quickstart-cmake.md
"""

import sys
from pathlib import Path

from config import exec, is_macos, is_windows, DIR_BUILD_VENDOR

BUILD_TYPE = "Debug"
CC = "clang"
C_STANDARD = "99"
CXX = "clang++"
CXX_STANDARD = "17"

DIR_SRC = Path(DIR_BUILD_VENDOR / "dawn")
DIR_CODE = Path(DIR_SRC / "code")
DIR_BUILD = Path(DIR_SRC, "build")
DIR_INSTALL = Path(DIR_SRC, "install")

GIT_SOURCE_URL = "https://dawn.googlesource.com/dawn"
GIT_HASH = "a2afb6c"  # Revert "Reapply "[tint][fuzz] Add fuzzer corpora minimization""


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
        f" -DCMAKE_BUILD_TYPE={BUILD_TYPE}"
        # Library specific options
        " -DDAWN_FETCH_DEPENDENCIES=ON"
        " -DDAWN_ENABLE_INSTALL=ON"
        " -DDAWN_BUILD_MONOLITHIC_LIBRARY=ON"
        " -DDAWN_FETCH_DEPENDENCIES=ON"
        " -DDAWN_ENABLE_INSTALL=ON"
        " -DDAWN_BUILD_SAMPLES=OFF"
        " -DTINT_BUILD_TESTS=OFF"
        " -DTINT_BUILD_FUZZERS=OFF"
        " -DTINT_BUILD_BENCHMARKS=OFF"
        " -DTINT_BUILD_AS_OTHER_OS=OFF"
    )

    if is_windows():
        command = (
            "cmake"
            f" -DCMAKE_C_COMPILER={CC}"
            f" -DCMAKE_C_STANDARD={C_STANDARD}"
            " -DCMAKE_C_STANDARD_REQUIRED=ON"
            f" -DCMAKE_CXX_COMPILER={CXX}"
            f" -DCMAKE_CXX_STANDARD={CXX_STANDARD}"
            " -DCMAKE_CXX_STANDARD_REQUIRED=ON"
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
        f" --config {BUILD_TYPE}"
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
