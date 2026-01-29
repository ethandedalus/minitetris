#pragma once

#include <raylib.h>

#include "Traits.h"
#include "Types.h"

struct Position {
  constexpr Position() noexcept : x(0), y(0) {}
  constexpr Position(i32 x, i32 y) noexcept : x(x), y(y) {}
  constexpr Position(Vector2 const& v) noexcept : x(static_cast<int>(v.x)), y(static_cast<int>(v.y)) {}

  constexpr explicit inline operator Vector2() const noexcept {
    return Vector2{.x = static_cast<float>(x), .y = static_cast<float>(y)};
  }

  constexpr inline bool operator==(Position const& other) const noexcept {
    return x == other.x && y == other.y;
  }

  i32 x;
  i32 y;
};

static_assert(Copy<Position>, "position should be trivially copyable");
