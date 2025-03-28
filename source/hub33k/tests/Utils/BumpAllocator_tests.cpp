#include <gtest/gtest.h>
#include <hub33k/Utils/BumpAllocator.hpp>

#include <print>

namespace hub33k {

  TEST(BumpAllocatorTest, Allocation) {
    BumpAllocator allocator(1024);
    void *ptr1 = allocator.Allocate(256);
    void *ptr2 = allocator.Allocate(512);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_EQ(allocator.AllocatedMemory(), 768);
  }

  TEST(BumpAllocatorTest, AllocationOverflow) {
    BumpAllocator allocator(1024);
    allocator.Allocate(1024);
    EXPECT_THROW(allocator.Allocate(1), std::bad_alloc);
  }

  TEST(BumpAllocatorTest, Reset) {
    BumpAllocator allocator(1024);
    allocator.Allocate(512);
    allocator.Reset();
    EXPECT_EQ(allocator.AllocatedMemory(), 0);
    void *ptr = allocator.Allocate(1024);
    EXPECT_NE(ptr, nullptr);
  }

  TEST(BumpAllocatorTest, EdgeCases) {
    BumpAllocator allocator(1024);
    EXPECT_THROW(allocator.Allocate(2048), std::bad_alloc);
    allocator.Allocate(0);
    EXPECT_EQ(allocator.AllocatedMemory(), 0);
  }

  TEST(BumpAllocatorTest, Alignment) {
    BumpAllocator allocator(1024);

    void *ptr1 = allocator.Allocate(1, 16);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr1) % 16, 0);

    void *ptr2 = allocator.Allocate(1, 32);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr2) % 32, 0);

    void *ptr3 = allocator.Allocate(1, 64);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr3) % 64, 0);
  }

  // ================================================================

  TEST(BumpAllocatorTest, GameExample) {
    GTEST_SKIP_("Skipped. Testing stuff");

    struct GameObject {
      int x, y;
      float velocity;
    };

    BumpAllocator frameAllocator(1024 * 1024); // 1MB per frame
    size_t objectCount = 100;
    GameObject *objects = static_cast<GameObject *>(frameAllocator.Allocate(objectCount * sizeof(GameObject)));

    for (size_t i = 0; i < objectCount; ++i) {
      objects[i] = {
        .x = 0,
        .y = 0,
        .velocity = 0,
      };
    }

    std::println("Allocated memory: {}", frameAllocator.AllocatedMemory());
    std::println("Available memory: {}", frameAllocator.AvailableMemory());
    std::println("Current offset: {}", frameAllocator.CurrentOffset());

    frameAllocator.Reset();

    std::println("Allocated memory: {}", frameAllocator.AllocatedMemory());
    std::println("Available memory: {}", frameAllocator.AvailableMemory());
    std::println("Current offset: {}", frameAllocator.CurrentOffset());
  }

  TEST(BumpAllocator, Sandbox) {
    GTEST_SKIP_("Skipped. Testing stuff");

    using namespace std;

    BumpAllocator ba(16);
    void *ptr1 = ba.Allocate(4);
    void *ptr2 = ba.Allocate(2);

    *(reinterpret_cast<int *>(ptr1)) = 22;
    *(reinterpret_cast<int *>(ptr2)) = 42;

    println("ptr1: {}", ptr1);
    println("ptr2: {}", ptr2);
    println("ptr1 value: {}", *(reinterpret_cast<int *>(ptr1)));
    println("ptr2 value: {}", *(reinterpret_cast<int *>(ptr2)));

    println("Available memory: {}", ba.AvailableMemory());
    println("Allocated memory: {}", ba.AllocatedMemory());
    println("Current offset: {}", ba.CurrentOffset());
  }

} // namespace hub33k
