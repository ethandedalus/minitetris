#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(Config config) noexcept
    : m_config(config) {
}

AudioPlayer::~AudioPlayer() noexcept {
  StopAudioStream(m_stream);
  UnloadAudioStream(m_stream);
  CloseAudioDevice();
}

void AudioPlayer::Play() noexcept {
  m_paused = false;
  PlayAudioStream(m_stream);
}

void AudioPlayer::Pause() noexcept {
  m_paused = true;
  PauseAudioStream(m_stream);
}

void AudioPlayer::Open() noexcept {
  m_stream = LoadAudioStream(m_config.sample_rate, m_config.sample_size, m_config.channels);
}
