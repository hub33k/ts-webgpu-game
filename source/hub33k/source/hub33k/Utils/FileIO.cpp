#include "FileIO.hpp"

#include "hub33k/Core/Log.hpp"
#include <filesystem>
#include <fstream>

#if defined(CopyFile)
  #undef CopyFile
#endif

namespace hub33k {

  long long FileIO::GetFileTimestamp(const std::string &filePath) {
    using namespace std::filesystem;
    using namespace std::chrono;

    if (std::filesystem::exists(filePath) == false) {
      HK_LOG_WARN("File does not exist: {0}", filePath);
      return -1;
    }

    auto ftime = last_write_time(filePath);

    // Convert file_time to system_clock::time_point
    auto sctp = time_point_cast<system_clock::duration>(ftime - file_time_type::clock::now() + system_clock::now());

    // Convert to time_t
    return system_clock::to_time_t(sctp);
  }

  long FileIO::GetFileSize(const std::string &filePath) {
    namespace fs = std::filesystem;

    if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
      HK_LOG_WARN("File does not exist or is not a regular file: {0}", filePath);
      return 0;
    }

    return static_cast<long>(fs::file_size(filePath));
  }

  bool FileIO::IsFileExists(const std::string &filePath) { return std::filesystem::exists(filePath); }

  std::string FileIO::ReadFile(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary); // Open in binary mode to handle all file types

    if (!file) {
      // TODO (hub33k): create custom exceptions
      throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::ostringstream ss;
    ss << file.rdbuf(); // Read file into stream buffer
    return ss.str();
  }

  void FileIO::WriteFile(const std::string &filePath, const std::string &content) {
    std::ofstream file(filePath, std::ios::binary); // Open in binary mode to handle all file types
    file << content;
    file.close();
  }

  void FileIO::CopyFile(const std::string &sourcePath, const std::string &destinationPath) {
    std::filesystem::copy(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
  }

} // namespace hub33k
