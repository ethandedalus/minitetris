#pragma once

#include <format>

#include "DesignSystem.h"
#include "Position.h"
#include "Types.h"

enum class Piece {
  I,
  O,
  T,
  S,
  Z,
  J,
  L,
};

enum CellType {
  CELL_EMPTY,
  CELL_CYAN,     // I
  CELL_YELLOW,   // O
  CELL_MAGENTA,  // T
  CELL_GREEN,    // S
  CELL_RED,      // Z
  CELL_BLUE,     // J
  CELL_ORANGE,   // L
};

template <>
struct std::formatter<CellType> : std::formatter<std::string_view> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  template <typename TFormatContext>
  auto format(CellType const& ct, TFormatContext& ctx) const {
    switch (ct) {
      case CELL_CYAN:
        return std::format_to(ctx.out(), "{}", "\x1b[38;2;0;255;255m██\x1b[0m");
        break;
      case CELL_BLUE:
        return std::format_to(ctx.out(), "{}", "\x1b[38;2;0;0;255m██\x1b[0m");
        break;
      case CELL_ORANGE:
        return std::format_to(ctx.out(), "{}", "\x1b[38;2;255;165;0m██\x1b[0m");
        break;
      case CELL_YELLOW:
        return std::format_to(ctx.out(), "{}", "\x1b[38;2;255;255;0m██\x1b[0m");
        break;
      case CELL_GREEN:
        return std::format_to(ctx.out(), "\x1b[38;2;0;255;0m██\x1b[0m");
        break;
      case CELL_RED:
        return std::format_to(ctx.out(), "{}", "\x1b[38;2;255;0;0m██\x1b[0m");
        break;
      case CELL_MAGENTA:
        return std::format_to(ctx.out(), "{}", "\x1b[38;2;255;0;255m██\x1b[0m");
        break;
      default:
        return std::format_to(ctx.out(), "{}", "‧‧");
        break;
    };
  }
};

constexpr CellType PIECE_CELL_TYPE[] = {
    CELL_CYAN,     // I
    CELL_YELLOW,   // O
    CELL_MAGENTA,  // T
    CELL_GREEN,    // S
    CELL_RED,      // Z
    CELL_BLUE,     // J
    CELL_ORANGE,   // L
};

constexpr Color PIECE_COLOR[] = {
    TETRIS_CYAN,
    TETRIS_YELLOW,
    TETRIS_MAGENTA,
    TETRIS_GREEN,
    TETRIS_RED,
    TETRIS_BLUE,
    TETRIS_ORANGE,
};

struct Tetronimo {
  Piece    ty;
  usize    rotation;
  Position pos;
};

using TetronimoVariant = Array<Position, 4>;
using TetronimoSpec    = Array<Array<Position, 4>, 4>;
using TetronimoSpecMap = Array<TetronimoSpec, 7>;

constexpr TetronimoVariant I_0 = {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}};
constexpr TetronimoVariant I_1 = {{{2, 0}, {2, 1}, {2, 2}, {2, 3}}};
constexpr TetronimoVariant I_2 = {{{0, 2}, {1, 2}, {2, 2}, {3, 2}}};
constexpr TetronimoVariant I_3 = {{{1, 0}, {1, 1}, {1, 2}, {1, 3}}};

constexpr TetronimoSpec I_PIECE = {{I_0, I_1, I_2, I_3}};

constexpr TetronimoVariant O_0 = {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant O_1 = {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant O_2 = {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant O_3 = {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}};

constexpr TetronimoSpec O_PIECE = {{O_0, O_1, O_2, O_3}};

constexpr TetronimoVariant T_0 = {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant T_1 = {{{1, 0}, {1, 1}, {1, 2}, {2, 1}}};
constexpr TetronimoVariant T_2 = {{{0, 1}, {1, 1}, {2, 1}, {1, 2}}};
constexpr TetronimoVariant T_3 = {{{1, 0}, {1, 1}, {1, 2}, {0, 1}}};

constexpr TetronimoSpec T_PIECE = {{T_0, T_1, T_2, T_3}};

constexpr TetronimoVariant S_0 = {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}};
constexpr TetronimoVariant S_1 = {{{1, 0}, {1, 1}, {2, 1}, {2, 2}}};
constexpr TetronimoVariant S_2 = {{{1, 1}, {2, 1}, {0, 2}, {1, 2}}};
constexpr TetronimoVariant S_3 = {{{0, 0}, {0, 1}, {1, 1}, {1, 2}}};

constexpr TetronimoSpec S_PIECE = {{S_0, S_1, S_2, S_3}};

constexpr TetronimoVariant Z_0 = {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant Z_1 = {{{2, 0}, {2, 1}, {1, 1}, {1, 2}}};
constexpr TetronimoVariant Z_2 = {{{0, 1}, {1, 1}, {1, 2}, {2, 2}}};
constexpr TetronimoVariant Z_3 = {{{1, 0}, {1, 1}, {0, 1}, {0, 2}}};

constexpr TetronimoSpec Z_PIECE = {{Z_0, Z_1, Z_2, Z_3}};

constexpr TetronimoVariant J_0 = {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant J_1 = {{{1, 0}, {2, 0}, {1, 1}, {1, 2}}};
constexpr TetronimoVariant J_2 = {{{0, 1}, {1, 1}, {2, 1}, {2, 2}}};
constexpr TetronimoVariant J_3 = {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}};

constexpr TetronimoSpec J_PIECE = {{J_0, J_1, J_2, J_3}};

constexpr TetronimoVariant L_0 = {{{0, 1}, {1, 1}, {2, 0}, {2, 1}}};
constexpr TetronimoVariant L_1 = {{{1, 0}, {1, 1}, {1, 2}, {2, 2}}};
constexpr TetronimoVariant L_2 = {{{0, 1}, {0, 2}, {1, 1}, {2, 1}}};
constexpr TetronimoVariant L_3 = {{{0, 0}, {1, 0}, {1, 1}, {1, 2}}};

constexpr TetronimoSpec L_PIECE = {{L_0, L_1, L_2, L_3}};

constexpr TetronimoSpecMap PIECES = {{I_PIECE, O_PIECE, T_PIECE, S_PIECE, Z_PIECE, J_PIECE, L_PIECE}};
