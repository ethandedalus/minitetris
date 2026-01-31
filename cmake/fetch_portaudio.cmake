set(PORTAUDIO_VERSION v19.7.0)

include(FetchContent)

FetchContent_Declare(
  portaudio
  DOWNLOAD_EXTRACT_TIMESTAMP_OFF
  URL https://github.com/PortAudio/portaudio/archive/refs/tags/${PORTAUDIO_VERSION}.tar.gz
                                  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(portaudio)
