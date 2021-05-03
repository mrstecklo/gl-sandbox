#pragma once

#include "GLCPP/Texture.h"
#include <string>

namespace Image {

GL::Texture2D LoadBMP(const char* path);
GL::Texture2D LoadDDS(const char* path, uint32_t* mipmapCount);
GL::Texture2D Load(const std::string& s, uint32_t* mipmapCount, bool* invertV);

} // namespace Image
