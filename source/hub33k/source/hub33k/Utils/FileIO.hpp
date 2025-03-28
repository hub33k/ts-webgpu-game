#pragma once

/*
# FileIO

- file utils

# Resources

- https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/OpenGL/OpenGLShader.cpp#L136
- https://github.com/Cakez77/SchnitzelMotor/blob/master/src/schnitzel_lib.h#L883

*/

#include <string>

// TODO (hub33k): should it throw runtime_error exceptions or return something else?
// TODO (hub33k): add BumpAllocator to manage memory

namespace hub33k {

  class FileIO {
  public:
    /**
     * @brief Gets the timestep of a file.
     * @param filePath Path to the file.
     * @return long long representing the file timestamp. If the file does not exist, returns -1.
     */
    static long long GetFileTimestamp(const std::string &filePath);

    static long GetFileSize(const std::string &filePath);

    static bool IsFileExists(const std::string &filePath);

    // TODO (hub33k): implement Read File Line by Line (Efficient for Large Files)
    static std::string ReadFile(const std::string &filePath);

    static void WriteFile(const std::string &filePath, const std::string &content);

    /**
     * @brief Copies a file from one location to another.
     * @param sourcePath Path to the source file.
     * @param destinationPath Path to the destination file.
     */
    static void CopyFile(const std::string &sourcePath, const std::string &destinationPath);
  };

} // namespace hub33k
