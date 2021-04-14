#pragma once

#include "GLTexture.h"

namespace Image {

GL::Texture2D LoadBMP(const char* path);
GL::Texture2D LoadDDS(const char* path, uint32_t* mipmapCount);

} // namespace Image