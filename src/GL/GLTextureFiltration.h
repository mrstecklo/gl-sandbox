#pragma once

#include <GL/glew.h>

namespace GL {

enum TextureGenericFilter : GLint {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

enum TextureMipMapFilter : GLint {
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

class TextureFilter {
public:
    constexpr GLint Get() const { return value; }
protected:
    constexpr TextureFilter(GLint v) : value(v) {}
private:
    GLint value;
};

class TextureMinFilter : public TextureFilter {
public:
    constexpr TextureMinFilter(TextureGenericFilter v) : TextureFilter(v) {}
    constexpr TextureMinFilter(TextureMipMapFilter v) : TextureFilter(v) {}
};

class TextureMagFilter : public TextureFilter {
public:
    constexpr TextureMagFilter(TextureGenericFilter v) : TextureFilter(v) {}
};

} // namespace GL