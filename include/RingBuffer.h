#pragma once

#include "Types.h"

template <typename T, usize SIZE>
class RingBuffer {
public:
  template <typename... Args>
  RingBuffer(Args&&... args) : m_buffer{std::forward<Args>(args)...}, m_head(0) {}

  T Cycle(T&& item) noexcept {
    T old            = std::move(m_buffer[m_head]);
    m_buffer[m_head] = std::move(item);
    m_head           = (m_head + 1) % SIZE;
    return old;
  }

  T const& operator[](usize i) const noexcept {
    return m_buffer[(m_head + i) % SIZE];
  }

  static constexpr usize Size() noexcept { return SIZE; }

private:
  std::array<T, SIZE> m_buffer;
  usize               m_head;
};
