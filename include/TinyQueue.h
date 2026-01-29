#pragma once

#include <array>

#include "Types.h"

template <typename T, usize SIZE>
class TinyQueue {
public:
  TinyQueue() noexcept : m_r(0), m_w(0), m_size(0) {};

  template <typename... Args>
    requires(sizeof...(Args) >= 1 && sizeof...(Args) <= SIZE && (std::is_convertible_v<Args, T> && ...))
  TinyQueue(Args&&... args) noexcept
      : m_buffer{std::forward<Args>(args)...}, m_size(sizeof...(Args)), m_r(0), m_w(sizeof...(Args) % SIZE) {}

  void Enqueue(T&& item) noexcept {
    if (m_size == SIZE) {
      return;
    }

    m_buffer[m_w] = std::move(item);
    m_w           = (m_w + 1) % SIZE;
    m_size++;
  }

  Option<T> Dequeue() noexcept {
    if (m_size == 0) {
      return std::nullopt;
    }

    T val = std::move(m_buffer[m_r]);
    m_r   = (m_r + 1) % SIZE;
    m_size--;
    return val;
  }

private:
  std::array<T, SIZE> m_buffer;

  usize m_size;
  usize m_r;
  usize m_w;
};
