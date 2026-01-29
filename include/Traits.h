#pragma once

#include <format>
#include <type_traits>

#include "Types.h"

template <typename T>
concept Copy = std::is_trivially_copyable_v<T>;

template <typename T>
concept Display =
    requires(const T& t) {
      std::format("{}", t);
    };

template <typename G, typename T, usize ROWS, usize COLS>
concept Grid2D =
    requires(G& g, const G& cg, usize i, usize j, usize row, usize col) {
      { g[i, j] } -> std::same_as<T&>;
      { cg[i, j] } -> std::same_as<const T&>;
      // { cg.GetRows() } -> std::same_as<usize>;
      // { cg.GetCols() } -> std::same_as<usize>;
      { cg.GetRow(row) } -> std::same_as<std::array<T, COLS>>;
      { cg.GetCol(col) } -> std::same_as<std::array<T, ROWS>>;
    };

// constexpr inline auto GetRows() const noexcept -> usize { return ROWS; }
// constexpr inline auto GetColumns() const noexcept -> usize { return COLS; }
//
// constexpr auto GetRow(usize row) const noexcept -> std::array<T, COLS> {
//   std::array<T, COLS> out{};
//   std::copy_n(m_inner.data() + row * COLS, COLS, out.begin());
//
//   return out;
// }
