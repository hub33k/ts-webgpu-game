#!/usr/bin/env python3

import argparse
from pathlib import Path
import subprocess
from shlex import split

# TODO (hub33k): parse CMakePresets.json and CMakeUserPresets.json

ROOT_DIR = Path(__file__).parent
BUILD_DIR = ROOT_DIR / "build-debug"

parser = argparse.ArgumentParser(
    prog="build",
    description="Build helpers",
    epilog="Created by hub33k",
)

parser.add_argument(
    "command",
    choices=[
        "configure",
        "build",
        "clean",
        "rebuild",
        "test",
        "run",
    ],
    help="Command to execute",
)

parser.add_argument("--debug", action="store_true")
parser.add_argument("--release", action="store_true")
parser.add_argument("--web", action="store_true")
parser.add_argument("-v", "--verbose", action="store_true")

args = parser.parse_args()

is_debug = args.debug
is_release = args.release
is_web = args.web
is_verbose = args.verbose


def prepare():
    BUILD_DIR.mkdir(parents=True, exist_ok=True)


# cmake --list-presets
def configure():
    command = "cmake --preset debug"

    if is_web:
        command = "emcmake cmake -G Ninja --preset debug-emscripten"

    print(f"[command] {command}")
    # on mac - shell=False, on windows - shell=True?
    subprocess.run(split(command), shell=False, check=True, cwd=ROOT_DIR)


def build():
    command = "cmake --build ./build-debug --target game"

    if is_web:
        command = "cmake --build ./build-debug-emscripten --target game"

    print(f"[command] {command}")
    subprocess.run(split(command), check=True, cwd=ROOT_DIR)


def run():
    command = "./build-debug/bin/gamed"
    print(f"[command] {command}")
    subprocess.run(split(command), check=True, cwd=ROOT_DIR)


def main():
    # prepare()

    # Base commands
    if args.command == "configure":
        configure()
    elif args.command == "build":
        build()
    elif args.command == "clean":
        pass
    elif args.command == "rebuild":
        pass
    elif args.command == "test":
        pass
    elif args.command == "run":
        run()


if __name__ == "__main__":
    main()
