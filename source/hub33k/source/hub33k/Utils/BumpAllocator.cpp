#include "BumpAllocator.hpp"

namespace hub33k {

  BumpAllocator::BumpAllocator(std::size_t size) : m_Size(size), m_Offset(0) {
    m_Memory = std::make_unique<std::byte[]>(size);
  }

  BumpAllocator::~BumpAllocator() {
  }

  void *BumpAllocator::Allocate(std::size_t size, std::size_t alignment) {
    std::size_t current_address = reinterpret_cast<std::size_t>(m_Memory.get() + m_Offset);
    std::size_t aligned_address = (current_address + alignment - 1) & ~(alignment - 1);
    std::size_t padding = aligned_address - current_address;

    if (m_Offset + padding + size > m_Size) {
      throw std::bad_alloc();
    }
    m_Offset += padding;

    void *ptr = m_Memory.get() + m_Offset;
    m_Offset += size;
    return ptr;
  }

  void BumpAllocator::Reset() {
    m_Offset = 0;
  }

  std::size_t BumpAllocator::Size() const {
    return m_Size;
  }

  std::size_t BumpAllocator::AllocatedMemory() const {
    return m_Offset;
  }

  std::size_t BumpAllocator::AvailableMemory() const {
    return m_Size - m_Offset;
  }

  std::size_t BumpAllocator::CurrentOffset() const {
    return m_Offset;
  }

  std::size_t BumpAllocator::CurrentSize() const {
    return m_Size;
  }

} // namespace hub33k
