#pragma once

#include <GL/glew.h>

namespace GL {

enum FloatFormat : GLint {
    RGBA32F = GL_RGBA32F,
    RGBA16F = GL_RGBA16F,
    RG32F = GL_RG32F,
    RG16F = GL_RG16F,
    R32F = GL_R32F,
    R16F = GL_R16F,
    RGB32F = GL_RGB32F,
    RGB16F = GL_RGB16F
};

enum DepthStencilFormat : GLint {
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
};

enum DepthStencilFloatFormat : GLint {
    DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
    DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8
};

enum RGTCFormat : GLint {
    COMPRESSED_RED_RGTC1 = GL_COMPRESSED_RED_RGTC1,
    COMPRESSED_SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
    COMPRESSED_RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
    COMPRESSED_SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2
};

enum S3TCFormat : GLint {
    COMPRESSED_RGBA_S3TC_DXT1_EXT = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
    COMPRESSED_RGBA_S3TC_DXT3_EXT = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
    COMPRESSED_RGBA_S3TC_DXT5_EXT = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};

enum PackedFloatFormat : GLint {
    R11F_G11F_B10F = GL_R11F_G11F_B10F
};

enum SharedExponentFormat : GLint {
    RGB9_E5 = GL_RGB9_E5
};

enum SignedIntegerFormat : GLint {
    RGBA32I = GL_RGBA32I,
    RGBA16I = GL_RGBA16I,
    RG32I = GL_RG32I,
    RGB16I = GL_RGB16I,
    RG8I = GL_RG8I,
    R32I = GL_R32I,
    R16I = GL_R16I,
    R8I = GL_R8I,
    RGB32I = GL_RGB32I,
    RGB8I = GL_RGB8I
};

enum UnsignedIntegerFormat : GLint {
    RGBA32UI = GL_RGBA32UI,
    RGBA16UI = GL_RGBA16UI,
    RGBA8UI = GL_RGBA8UI,
    RG32UI = GL_RG32UI,
    RGB16UI = GL_RGB16UI,
    RG8UI = GL_RG8UI,
    R32UI = GL_R32UI,
    R16UI = GL_R16UI,
    R8UI = GL_R8UI,
    RGB32UI = GL_RGB32UI,
    RGB8UI = GL_RGB8UI
};

enum UnsignedNormalizedFormat : GLint {
    RGBA16 = GL_RGBA16,
    RGBA8 = GL_RGBA8,
    RG16 = GL_RG16,
    RG8 = GL_RG8,
    R8 = GL_R8,
    RGB16 = GL_RGB16,
    RGB8 = GL_RGB8
};

enum SignedNormalizedFormat : GLint {
    RGBA16_SNORM = GL_RGBA16_SNORM,
    RGBA8_SNORM = GL_RGBA8_SNORM,
    RGB16_SNORM = GL_RGB16_SNORM,
    RGB8_SNORM = GL_RGB8_SNORM,
    RG16_SNORM = GL_RG16_SNORM,
    RG8_SNORM = GL_RG8_SNORM,
    R16_SNORM = GL_R16_SNORM,
    R8_SNORM = GL_R8_SNORM 
};

enum PackedUnsignedFormat : GLint {
    RGB10_A2UI = GL_RGB10_A2UI
};

enum PackedNormalizedFormat : GLint {
    RGB10_A2 = GL_RGB10_A2
};

enum sRGBFormat : GLint {
    SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
    SRGB8 = GL_SRGB8
};

class FormatBase {
public:
    constexpr GLint Get() const { return value; }
protected:
    constexpr FormatBase(GLint v) : value(v) {}
private:
    GLint value;
};

class CompressedFormat : public FormatBase {
public:
    CompressedFormat(RGTCFormat v)      : FormatBase(v) {}
    CompressedFormat(S3TCFormat v)      : FormatBase(v) {}
};

class Format : public FormatBase {
public:
    Format(FloatFormat v)               : FormatBase(v) {}
    Format(DepthStencilFormat v)        : FormatBase(v) {}
    Format(DepthStencilFloatFormat v)   : FormatBase(v) {}
    Format(PackedFloatFormat v)         : FormatBase(v) {}
    Format(SharedExponentFormat v)      : FormatBase(v) {}
    Format(UnsignedIntegerFormat v)     : FormatBase(v) {}
    Format(SignedIntegerFormat v)       : FormatBase(v) {}
    Format(UnsignedNormalizedFormat v)  : FormatBase(v) {}
    Format(SignedNormalizedFormat v)    : FormatBase(v) {}
    Format(PackedUnsignedFormat v)      : FormatBase(v) {}
    Format(PackedNormalizedFormat v)    : FormatBase(v) {}
    Format(sRGBFormat v)                : FormatBase(v) {}

    Format(CompressedFormat v)          : FormatBase(v) {}
};

} // namespace GL