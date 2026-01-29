#pragma once

#include <format>
#include <type_traits>

template <typename T>
concept Copy = std::is_trivially_copyable_v<T>;

template <typename T>
concept Display =
    requires(const T& t) {
      std::format("{}", t);
    };
