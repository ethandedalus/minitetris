#include "Timing.h"

Timing::Timing() noexcept : m_now(Timing::SteadyClock::now()) {}

void Timing::Update() noexcept {
  m_now = SteadyClock::now();
}

bool Timing::Tick(i32 id) noexcept {
  auto& timer = m_timers[id];
  if (m_now - timer.last_tick >= timer.interval) {
    timer.last_tick = m_now;
    return true;
  }
  return false;
}

bool Timing::CanRepeatKey(i32 key, Duration delay) noexcept {
  if (m_now - m_key_times[key] >= delay) {
    m_key_times[key] = m_now;
    return true;
  }
  return false;
}
