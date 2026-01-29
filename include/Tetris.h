#pragma once

#include <cassert>
#include <format>
#include <random>

#include "Tetronimo.h"
#include "Timing.h"
#include "TinyQueue.h"
#include "Types.h"

enum class GameState {
  Start,
  Running,
  Paused,
  GameOver,
};

class Tetris {
  friend struct std::formatter<Tetris>;

public:
  struct Config {
    usize start_w, start_h;
  };

  Tetris(Config const& config) noexcept;

  void Run() noexcept;

private:
  enum TimerID {
    TIMER_DROP,
  };

  constexpr static usize              ROWS         = 20;
  constexpr static usize              COLS         = 10;
  constexpr static Array<Position, 4> KICK_OFFSETS = {{
      {0, 0},   // in place
      {-1, 0},  // left
      {1, 0},   // right
      {0, -1},  // up (for floor kicks)
  }};

  void Draw() noexcept;

  void DrawStartOverlay() noexcept;
  void DrawGameOverOverlay() noexcept;
  void DrawPausedOverlay() noexcept;

  void Update() noexcept;

  constexpr Position GetOrigin() const noexcept;
  constexpr usize    GetUnitSize() const noexcept;

  constexpr CellType& operator[](usize i, usize j) noexcept {
    usize index = COLS * i + j;
    assert(index < ROWS * COLS && "index out of bounds");
    return m_grid[index];
  }

  constexpr CellType const& operator[](usize i, usize j) const noexcept {
    usize index = COLS * i + j;
    assert(index < ROWS * COLS && "index out of bounds");
    return m_grid[index];
  }

  constexpr bool        CanDrop() const noexcept;
  constexpr bool        CanShiftLeft() const noexcept;
  constexpr bool        CanShiftRight() const noexcept;
  constexpr bool        CanRotate() const noexcept;
  constexpr bool        Fits(Position const& pos, usize rotation) const noexcept;
  constexpr inline void Drop() noexcept { m_active_tetronimo.pos.y += 1; }
  constexpr inline void ShiftLeft() noexcept { m_active_tetronimo.pos.x -= 1; }
  constexpr inline void ShiftRight() noexcept { m_active_tetronimo.pos.x += 1; }
  constexpr void        WriteTetronimo() noexcept;
  constexpr bool        TryRotate() noexcept;
  constexpr void        Reset() noexcept;
  constexpr void        RemoveFilledRows() noexcept;

  inline i32 RandomInt(i32 min, i32 max) {
    return std::uniform_int_distribution<i32>{min, max}(m_rng);
  }

  inline Piece RandomPiece() {
    return static_cast<Piece>(RandomInt(static_cast<i32>(Piece::I), static_cast<i32>(Piece::L)));
  }

  Tetronimo RandomTetronimo() noexcept;

  Array<CellType, ROWS * COLS> m_grid{};
  TinyQueue<Tetronimo, 4>      m_piece_buffer;
  Tetronimo                    m_active_tetronimo;
  std::mt19937                 m_rng{std::random_device{}()};
  Config                       m_config;
  Timing                       m_timing;
  GameState                    m_game_state;
};

template <>
struct std::formatter<Tetris> : std::formatter<std::string_view> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  template <typename TFormatContext>
  auto format(Tetris const& tetris, TFormatContext& ctx) const {
    auto out = ctx.out();

    out = std::format_to(out, "┌");
    for (usize j = 0; j < Tetris::COLS; ++j) {
      out = std::format_to(out, "──");
    }
    out = std::format_to(out, "┐\n");

    for (usize i = 0; i < Tetris::ROWS; ++i) {
      out = std::format_to(out, "│");
      for (usize j = 0; j < Tetris::COLS; ++j) {
        out = std::format_to(out, "{}", tetris[i, j]);
      }
      out = std::format_to(out, "│\n");
    }

    out = std::format_to(out, "└");
    for (usize j = 0; j < Tetris::COLS; ++j) {
      out = std::format_to(out, "──");
    }
    out = std::format_to(out, "┘");

    return out;
  }
};
