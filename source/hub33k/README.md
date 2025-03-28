# `hub33k`

My utils for gamedev in C++.

## Requirements

- C++ 23
- libraries
  - spdlog v1.x - https://github.com/gabime/spdlog

## TODO

- [ ] t
- [ ] add switch to define that should be included in lib (eg. do not add BumpAllocator util)
- [ ] create self containing lib (hub33k lib do not need spdlog as lib)
  - contain spdlog inside lib?
- [ ] always define eg. HK_PLATFORM_WINDOWS and set it to 0 or 1?
- [ ] implement stuff from cpp-HubUtils - compiler, platform, HAL, arch, build type, etc.
  - disable warning for compilers macro
- [ ] create feature detection header (check cmake-init)
  - [ ] check for other code that implement the same thing (EA, Epic, check Defines.hpp)

## GameDev

- Bump Allocator
- File IO
- RAII
- SIMD
