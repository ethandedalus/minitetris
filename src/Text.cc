#include "Text.h"

#include "Resources.h"

Font& GetFont() noexcept {
  static Font font = LoadFontFromMemory(
      ".ttf",
      PIXELOID_MONO,
      sizeof(PIXELOID_MONO),
      256,
      nullptr,
      0);
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
  return font;
}
