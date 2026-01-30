#include "Text.h"

#include "Resources.h"

Font& GetFont() noexcept {
  static Font font = LoadFontFromMemory(
      ".ttf",
      assets::PIXELOID_MONO,
      sizeof(assets::PIXELOID_MONO),
      256,
      nullptr,
      0);
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
  return font;
}
