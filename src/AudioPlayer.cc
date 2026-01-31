#include "AudioPlayer.h"

#include "Resources.h"

AudioPlayer::AudioPlayer() noexcept : m_stream(nullptr), m_state{nullptr, 0, 0} {
  Pa_Initialize();
}

void AudioPlayer::SetPCMTrack(u8 const* data) noexcept {
  Stop();

  m_state.samples       = reinterpret_cast<const f32*>(data);
  m_state.total_frames  = sizeof(assets::MAIN_TRACK) / (sizeof(f32) * CHANNELS);
  m_state.current_frame = 0;
}

void AudioPlayer::LoopTrack() noexcept {
  Stop();
  Pa_OpenDefaultStream(
      &m_stream, 0, CHANNELS, paFloat32,
      SAMPLE_RATE, 256, [](const void*, void* output, usize frame_count, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* user_data) -> i32 {
        auto* state = static_cast<PlaybackState*>(user_data);
        auto* out   = static_cast<f32*>(output);

        for (usize i = 0; i < frame_count; ++i) {
          if (state->current_frame >= state->total_frames) {
            state->current_frame = 0;
          }
          *out++ = state->samples[state->current_frame * CHANNELS];
          *out++ = state->samples[state->current_frame * CHANNELS + 1];
          ++state->current_frame;
        }

        return paContinue;
      },
      &m_state);
  Pa_StartStream(m_stream);
}

void AudioPlayer::Pause() noexcept {
  if (m_stream && Pa_IsStreamActive(m_stream)) {
    Pa_StopStream(m_stream);
  }
}

void AudioPlayer::Resume() noexcept {
  if (m_stream && !Pa_IsStreamActive(m_stream)) {
    Pa_StartStream(m_stream);
  }
}

void AudioPlayer::Restart() noexcept {
  m_state.current_frame = 0;
  if (m_stream && !Pa_IsStreamActive(m_stream)) {
    Pa_StartStream(m_stream);
  }
}

void AudioPlayer::Stop() noexcept {
  if (m_stream) {
    Pa_StopStream(m_stream);
    Pa_CloseStream(m_stream);
    m_stream = nullptr;
  }
}
