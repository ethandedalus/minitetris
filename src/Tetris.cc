#include "Tetris.h"

#include <raylib.h>

#include <algorithm>

#include "DesignSystem.h"
#include "Resources.h"
#include "Text.h"

using namespace std::chrono_literals;

Tetris::Tetris(Config const& config) noexcept
    : m_config(config),
      m_rng{static_cast<u32>(std::chrono::steady_clock::now().time_since_epoch().count()) ^ std::random_device{}()},
      m_piece_buffer(
          RandomTetronimo(),
          RandomTetronimo(),
          RandomTetronimo()),
      m_active_tetronimo(RandomTetronimo()),
      m_game_state(GameState::Start),
      m_score(0),
      m_level(0) {
  m_timing.SetInterval(TIMER_DROP, 1000ms);
  m_timing.SetInterval(TIMER_LOCK, 500ms);
}

Tetronimo Tetris::RandomTetronimo() noexcept {
  auto piece = RandomPiece();
  auto pos   = RandomInt(3, 5);

  return Tetronimo{.ty = piece, .rotation = 0, .pos = {pos, 0}};
}

constexpr Position Tetris::GetOrigin() const noexcept {
  usize width  = GetScreenWidth();
  usize height = GetScreenHeight();
  if (width > height) {
    return {static_cast<i32>(width - height) / 2, 0};
  } else if (height > width) {
    return {0, static_cast<i32>(height - width) / 2};
  } else {
    return {0, 0};
  }
}

constexpr usize Tetris::GetUnitSize() const noexcept {
  usize width  = GetScreenWidth();
  usize height = GetScreenHeight();

  usize unit = width / (Tetris::ROWS + 2);
  if (width > height) {
    unit = height / (Tetris::ROWS + 2);
  }
  return unit;
}

constexpr bool Tetris::CanDrop() const noexcept {
  return Fits({m_active_tetronimo.pos.x, m_active_tetronimo.pos.y + 1}, m_active_tetronimo.rotation);
}

constexpr bool Tetris::CanShiftLeft() const noexcept {
  return Fits({m_active_tetronimo.pos.x - 1, m_active_tetronimo.pos.y}, m_active_tetronimo.rotation);
}

constexpr bool Tetris::CanShiftRight() const noexcept {
  return Fits({m_active_tetronimo.pos.x + 1, m_active_tetronimo.pos.y}, m_active_tetronimo.rotation);
}

constexpr bool Tetris::TryRotate() noexcept {
  usize new_rotation = (m_active_tetronimo.rotation + 1) % 4;

  for (auto [dx, dy] : KICK_OFFSETS) {
    Position candidate = {m_active_tetronimo.pos.x + dx, m_active_tetronimo.pos.y + dy};
    if (Fits(candidate, new_rotation)) {
      m_active_tetronimo.pos      = candidate;
      m_active_tetronimo.rotation = new_rotation;
      return true;
    }
  }

  return false;
}

void Tetris::DrawStartOverlay() noexcept {
  Vector2 origin     = static_cast<Vector2>(GetOrigin());
  f32     unit       = static_cast<f32>(GetUnitSize());
  Vector2 grid_start = {.x = origin.x + unit, .y = origin.y + unit};

  Rectangle overlay_rec = {.x = grid_start.x, .y = grid_start.y, .width = unit * (COLS * 2), .height = unit * ROWS};

  DrawRectangleRec(overlay_rec, OVERLAY);

  f32 title_bounding_box_width = overlay_rec.width * 0.9;

  f32 max_text_width = title_bounding_box_width - 2 * unit;

  f32 font_size = 200.0F;

  Vector2 dimensions = GetTextDimensions("TETRIS", font_size, 2.0F);
  while (dimensions.x > max_text_width) {
    font_size -= 1;
    dimensions = GetTextDimensions("TETRIS", font_size, 2.0F);
  }

  Rectangle bounding_box = {.x = grid_start.x + unit, .y = grid_start.y + unit, .width = title_bounding_box_width, .height = dimensions.y};

  DrawRectangleRoundedLinesEx(bounding_box, 0.05F, 0, 2, FOREGROUND);

  f32 offset = bounding_box.x + unit;

  // font is monospaced so this works
  Vector2 letter_dimensions = GetTextDimensions("T", font_size, 2.0F);
  for (usize i = 0; auto const& c : "TETRIS") {
    WriteText(std::format("{}", c).c_str(),
              (Vector2){
                  .x = static_cast<float>(offset),
                  .y = overlay_rec.y + unit,
              },
              font_size, 2.0F, PIECE_COLOR[i]);
    offset += letter_dimensions.x + 2.0F;
    i += 1;
  }

  font_size                           = 50;
  f32 start_button_bounding_box_width = overlay_rec.width * 0.4;
  f32 max_start_button_text_width     = start_button_bounding_box_width - 2 * 0.05 * start_button_bounding_box_width;

  f32 start_button_font_size = 50.0F;
  dimensions                 = GetTextDimensions("PRESS SPACE TO START", font_size, 2.0F);

  while (dimensions.x > max_start_button_text_width) {
    font_size -= 1;
    dimensions = GetTextDimensions("PRESS SPACE TO START", font_size, 2.0F);
  }

  bounding_box = {
      .x      = (grid_start.x + (overlay_rec.width - start_button_bounding_box_width) / 2),
      .y      = grid_start.y + overlay_rec.height / 2,
      .width  = start_button_bounding_box_width,
      .height = dimensions.y * 2,
  };
  DrawRectangleRoundedLinesEx(bounding_box, 0.1F, 0, 2, FOREGROUND);

  WriteText("PRESS SPACE TO START",
            (Vector2){
                .x = static_cast<float>(bounding_box.x + 0.05 * bounding_box.width),
                .y = static_cast<float>(bounding_box.y + 0.25 * bounding_box.height),
            },
            font_size, 2.0F, FOREGROUND);
}

void Tetris::DrawPausedOverlay() noexcept {}
void Tetris::DrawGameOverOverlay() noexcept {}

void Tetris::Draw() noexcept {
  Vector2   origin     = static_cast<Vector2>(GetOrigin());
  f32       unit       = static_cast<f32>(GetUnitSize());
  Vector2   grid_start = {.x = origin.x + unit, .y = origin.y + unit};
  Rectangle tetris_rec = {.x = grid_start.x, .y = grid_start.y, .width = unit * COLS, .height = unit * ROWS};

  ClearBackground(BACKGROUND);

  // draw active tetronimo (beneath the lines)

  if (m_game_state == GameState::Running || m_game_state == GameState::GameOver) {
    auto [x0, y0] = m_active_tetronimo.pos;
    usize index   = static_cast<usize>(m_active_tetronimo.ty);
    auto& cells   = PIECES[index][m_active_tetronimo.rotation];

    for (Position const& pos : cells) {
      auto      resolved_coords = static_cast<Vector2>(pos);
      Rectangle component_rec   = {
            .x      = grid_start.x + resolved_coords.x * unit + x0 * unit,
            .y      = grid_start.y + resolved_coords.y * unit + y0 * unit,
            .width  = unit,
            .height = unit,
      };

      DrawRectangleRec(component_rec, PIECE_COLOR[static_cast<usize>(m_active_tetronimo.ty)]);
    }
  }

  for (usize i = 0; i < m_grid.size(); ++i) {
    if (m_grid[i] != CELL_EMPTY) {
      auto      resolved_coords = static_cast<Vector2>(Position{static_cast<i32>(i % COLS), static_cast<i32>(i / COLS)});
      Rectangle component_rec   = {
            .x      = grid_start.x + resolved_coords.x * unit,
            .y      = grid_start.y + resolved_coords.y * unit,
            .width  = unit,
            .height = unit,
      };

      DrawRectangleRec(component_rec, PIECE_COLOR[m_grid[i] - 1]);
    }
  }

  // draw grid lines
  for (usize i = 1; i < COLS; ++i) {
    DrawLineEx(
        {.x = grid_start.x + i * unit, .y = grid_start.y},
        {.x = grid_start.x + i * unit, .y = grid_start.y + unit * ROWS},
        2.0F, OVERLAY);
  }

  for (usize i = 1; i < ROWS; ++i) {
    DrawLineEx(
        {.x = grid_start.x, .y = grid_start.y + i * unit},
        {.x = grid_start.x + unit * COLS, .y = grid_start.y + i * unit},
        2.0F, OVERLAY);
  }

  // draw grid

  DrawRectangleLinesEx(tetris_rec, 2.0F, FOREGROUND);
}

constexpr void Tetris::WriteTetronimo() noexcept {
  auto [x0, y0]      = m_active_tetronimo.pos;
  usize index        = static_cast<usize>(m_active_tetronimo.ty);
  auto& cells        = PIECES[index][m_active_tetronimo.rotation];
  auto& [a, b, c, d] = cells;
  for (auto const& [x, y] : cells) {
    Position resolved_coords = {x0 + x, y0 + y};
    usize    q               = resolved_coords.x + resolved_coords.y * COLS;
    m_grid[q]                = PIECE_CELL_TYPE[index];
  }
}

void Tetris::Update() noexcept {
  m_timing.Update();

  switch (m_game_state) {
    case GameState::Start:
      if (IsKeyDown(KEY_SPACE)) {
        m_game_state = GameState::Running;
      }
      break;
    case GameState::Running:

      if (IsKeyDown(KEY_LEFT) && m_timing.CanRepeatKey(KEY_LEFT, 100ms) && CanShiftLeft()) {
        ShiftLeft();
      } else if (IsKeyDown(KEY_RIGHT) && m_timing.CanRepeatKey(KEY_RIGHT, 100ms) && CanShiftRight()) {
        ShiftRight();
      } else if (IsKeyDown(KEY_UP) && m_timing.CanRepeatKey(KEY_UP, 250ms)) {
        TryRotate();
      }

      if (CanDrop()) {
        m_locking = false;
        if (m_timing.Tick(TIMER_DROP) || IsKeyDown(KEY_DOWN) && m_timing.CanRepeatKey(KEY_DOWN, 50ms)) {
          Drop();
        }
      } else {
        if (!m_locking) {
          m_locking = true;
          m_timing.Reset(TIMER_LOCK);
        }
        if (m_timing.Tick(TIMER_LOCK) || (!CanShiftLeft() && !CanShiftRight())) {
          WriteTetronimo();
          RemoveFilledRows();
          m_active_tetronimo = m_piece_buffer.Dequeue().value();
          m_piece_buffer.Enqueue(RandomTetronimo());
          if (!Fits(m_active_tetronimo.pos, m_active_tetronimo.rotation)) {
            m_game_state = GameState::GameOver;
          }
        }
      }

      break;
    case GameState::Paused:
      break;
    case GameState::GameOver:
      break;
  }
}

void Tetris::Run() noexcept {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(m_config.start_w, m_config.start_h, "MiniTetris");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    Update();

    BeginDrawing();
    switch (m_game_state) {
      case GameState::Start:
        DrawStartOverlay();
        break;
      case GameState::Running:
        Draw();
        break;
      case GameState::Paused:
        break;
      case GameState::GameOver:
        break;
    }

    EndDrawing();
  }

  CloseWindow();
}

constexpr bool Tetris::Fits(Position const& pos, usize rotation) const noexcept {
  usize index = static_cast<usize>(m_active_tetronimo.ty);
  auto& cells = PIECES[index][rotation];

  for (auto const& [x, y] : cells) {
    i32 nx = pos.x + x;
    i32 ny = pos.y + y;

    if (nx < 0 || nx >= static_cast<i32>(COLS) ||
        ny < 0 || ny >= static_cast<i32>(ROWS)) {
      return false;
    }

    if (m_grid[ny * COLS + nx] != CELL_EMPTY) {
      return false;
    }
  }
  return true;
}

constexpr void Tetris::Reset() noexcept {
  std::fill(m_grid.begin(), m_grid.end(), CELL_EMPTY);
}

constexpr void Tetris::RemoveFilledRows() noexcept {
  for (usize i = ROWS; i-- > 0;) {
    bool filled = std::all_of(
        m_grid.begin() + i * COLS,
        m_grid.begin() + (i + 1) * COLS,
        [](auto x) { return x != CELL_EMPTY; });

    if (filled) {
      for (usize j = i; j > 0; j--) {
        std::copy(
            m_grid.begin() + (j - 1) * COLS,
            m_grid.begin() + j * COLS,
            m_grid.begin() + j * COLS);
      }
      std::fill(m_grid.begin(), m_grid.begin() + COLS, CELL_EMPTY);

      i++;
    }
  }
}
