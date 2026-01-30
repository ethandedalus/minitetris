#pragma once

#include <raylib.h>

#include <utility>

Font& GetFont() noexcept;

template <typename... Args>
void WriteText(Args&&... args) noexcept {
  DrawTextEx(GetFont(), std::forward<Args>(args)...);
}

template <typename... Args>
Vector2 GetTextDimensions(Args&&... args) noexcept {
  return MeasureTextEx(GetFont(), std::forward<Args>(args)...);
}
