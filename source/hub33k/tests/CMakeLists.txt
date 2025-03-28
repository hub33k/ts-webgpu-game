set(target hub33k_tests)

# https://google.github.io/googletest/quickstart-cmake.html

enable_testing()

add_executable(
  ${target}
  # Utils
  Utils/BumpAllocator_tests.cpp
  Utils/FileIO_tests.cpp
)
target_link_libraries(
  ${target}
  hub33k
  spdlog_header_only
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${target})
