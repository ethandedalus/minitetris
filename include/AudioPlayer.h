#pragma once

#include <portaudio.h>

#include "Types.h"

constexpr static i32 SAMPLE_RATE = 44100;
constexpr static i32 CHANNELS    = 2;

struct PlaybackState {
  const float* samples;
  usize        total_frames;
  usize        current_frame;
};

class AudioPlayer {
public:
  AudioPlayer() noexcept;

  void SetPCMTrack(u8 const* data) noexcept;
  void LoopTrack() noexcept;
  void Pause() noexcept;
  void Resume() noexcept;
  void Restart() noexcept;
  void Stop() noexcept;

  inline bool IsPaused() const noexcept {
    return m_stream && !Pa_IsStreamActive(m_stream);
  }

  inline bool IsPlaying() const noexcept {
    return m_stream && Pa_IsStreamActive(m_stream);
  }

private:
  PaStream*     m_stream;
  PlaybackState m_state;
};
