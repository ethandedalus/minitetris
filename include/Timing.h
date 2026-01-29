#pragma once

#include <chrono>
#include <unordered_map>

#include "Types.h"

class Timing {
public:
  using SteadyClock = std::chrono::steady_clock;
  using Duration    = std::chrono::milliseconds;
  using TimePoint   = SteadyClock::time_point;

  Timing() noexcept;

  constexpr inline TimePoint Now() const noexcept { return m_now; };
  void                       Update() noexcept;

  constexpr inline void SetInterval(i32 id, Duration interval) noexcept {
    m_timers[id].interval = interval;
  }

  bool Tick(i32 id) noexcept;
  bool CanRepeatKey(i32 key, Duration delay) noexcept;

private:
  struct Timer {
    Duration  interval{};
    TimePoint last_tick{};
  };

  TimePoint                          m_now;
  std::unordered_map<i32, Timer>     m_timers;
  std::unordered_map<i32, TimePoint> m_key_times;
};
