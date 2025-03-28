#include <gtest/gtest.h>

// TODO (hub33k): implement BumpAllocatorRaw

class BumpAllocatorRaw {
public:
  BumpAllocatorRaw(size_t size) {
    m_Buffer = malloc(size);
  }

  ~BumpAllocatorRaw() {
    free(m_Buffer);
  }

  void Allocate(size_t size) {
  }

  void Reset() {
    m_Offset = 0;
  }

private:
  size_t m_Capacity;
  size_t m_Offset;
  void *m_Buffer;
};

TEST(BumpAllocatorRawTest, Example) {
  BumpAllocatorRaw bar(1024);
}
