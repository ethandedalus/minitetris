#pragma once

#include "Types.h"
#include "raylib.h"

struct AudioData {
  const u8* data;
  size_t    size;
  size_t    position;
};

class AudioPlayer {
public:
  struct Config {
    usize sample_rate;
    usize sample_size;
    usize channels;
  };

  AudioPlayer(Config = {44100, 32, 2}) noexcept;
  ~AudioPlayer() noexcept;

  template <typename F>
  void SetCallback(F&& f) noexcept {
    SetAudioStreamCallback(m_stream, std::forward<F>(f));
  }

  void        Open() noexcept;
  void        Play() noexcept;
  void        Pause() noexcept;
  inline bool IsPaused() const noexcept { return m_paused; }

private:
  AudioStream m_stream;
  Config      m_config;
  bool        m_paused;
};
