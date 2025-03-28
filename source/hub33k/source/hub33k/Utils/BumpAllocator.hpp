#pragma once

/*
# Bump Allocator (linear allocator, stack allocator, arena allocator, etc.)

- size in bytes
- memory alignment, offset and size
- bump allocator usage (gamedev)

# Resources

- https://jonahmgoldsmith.hashnode.dev/temporary-allocations-in-cc
- https://badlydrawnrod.github.io/posts/2021/12/30/monotonic_buffer_resource/
- https://github.com/Cakez77/CelesteClone/blob/master/src/schnitzel_lib.h#L160
- https://www.youtube.com/watch?v=S96DCaV5X2M&list=PLFAIgTeqcARmowCzcOMil78OxcPNsac70&index=7
- https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator
- https://os.phil-opp.com/allocator-designs/
*/

#include <cstddef>
#include <memory>

// TODO (hub33k): handle memory resize

namespace hub33k {

  /**
   * @class BumpAllocator
   * @brief A simple and efficient bump allocator for fast memory allocation.
   *
   * This allocator is designed for scenarios where a large number of small allocations
   * are needed and freeing individual allocations is not required. Typical use cases
   * include temporary allocations in game development.
   */
  class BumpAllocator {
  public:
    /**
     * @brief Constructs a BumpAllocator with the given size.
     * @param size The total size of the memory pool.
     */
    BumpAllocator(std::size_t size);

    ~BumpAllocator();

    /**
     * @brief Allocates memory of the given size with the specified alignment.
     * @param size The size of the memory to allocate.
     * @param alignment The alignment requirement.
     * @return A pointer to the allocated memory.
     * @throws std::bad_alloc if there is not enough memory left.
     */
    void *Allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));

    /**
     * @brief Resets the allocator, allowing all memory to be reused.
     */
    void Reset();

    /**
     * @brief Gets the total size of the memory pool.
     * @return The total size of the memory pool.
     */
    std::size_t Size() const;

    /**
     * @brief Gets the amount of memory currently allocated.
     * @return The amount of memory currently allocated.
     */
    std::size_t AllocatedMemory() const;

    /**
     * @brief Gets the amount of available memory.
     * @return The amount of available memory.
     */
    std::size_t AvailableMemory() const;

    /**
     * @brief Gets the current offset in the memory pool.
     * @return The current offset in the memory pool.
     */
    std::size_t CurrentOffset() const;

    /**
     * @brief Gets the current size of the memory pool.
     * @return The current size of the memory pool.
     */
    std::size_t CurrentSize() const;

  private:
    std::size_t m_Size;                    ///< The total size of the memory pool.
    std::size_t m_Offset;                  ///< The current offset in the memory pool.
    std::unique_ptr<std::byte[]> m_Memory; ///< The memory pool.
  };

} // namespace hub33k
