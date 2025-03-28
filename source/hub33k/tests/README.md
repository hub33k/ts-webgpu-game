# hub33k tests

- https://google.github.io/googletest/quickstart-cmake.html
- run `cmake --build .\build-debug-llvm\ --target hub33k_tests && .\build-debug-llvm\bin\hub33k_tests.exe`
- `.\build-debug-llvm\bin\hub33k_tests.exe --gtest_list_tests`
- `cmake --build .\build-debug-llvm\ --target hub33k_tests && .\build-debug-llvm\bin\hub33k_tests.exe --gtest_filter=FileIO.ReadFile1_BufferTooSmall`

## TODO

- [ ] t
- [ ] google test run specific test or test suite
  - https://stackoverflow.com/questions/12076072/how-to-run-specific-test-cases-in-googletest
