#include <gtest/gtest.h>
#include <hub33k/Utils/FileIO.hpp>

#include <filesystem>
#include <fstream>
#include <print>

namespace hub33k {

#pragma region GetFileTimestamp
  class GetFileTimestampTest : public testing::Test {
  protected:
    const std::string testFile = "testfile.bin";

  protected:
    virtual void SetUp() {
      std::ofstream out(testFile, std::ios::binary);
      out << "1234567890"; // 10 bytes
      out.close();
    }

    virtual void TearDown() { std::filesystem::remove(testFile); }
  };

  TEST_F(GetFileTimestampTest, GetFileTimestamp) {
    std::string filePath = "testfile.bin";
    int64_t timestamp = FileIO::GetFileTimestamp(filePath);
    EXPECT_NE(timestamp, -1);

    std::string nonexistentFile = "nonexistent.bin";
    int64_t nonexistentTimestamp = FileIO::GetFileTimestamp(nonexistentFile);
    EXPECT_EQ(nonexistentTimestamp, -1);
  }
#pragma endregion GetFileTimestamp

#pragma region ReadFile
  class ReadFileTest : public testing::Test {
  protected:
    const std::string testFile = "testfile.bin";

  protected:
    virtual void SetUp() {
      std::ofstream out(testFile, std::ios::binary);
      out << "1234567890"; // 10 bytes
      out.close();
    }

    virtual void TearDown() { std::filesystem::remove(testFile); }
  };

  TEST_F(ReadFileTest, ReadFile_Basic) {
    std::string filePath = "testfile.bin";
    std::string content = FileIO::ReadFile(filePath);
    EXPECT_EQ(content, "1234567890");

    std::string nonexistentFile = "nonexistent.bin";
    EXPECT_THROW(FileIO::ReadFile(nonexistentFile), std::runtime_error);
  }
#pragma endregion ReadFile

} // namespace hub33k
