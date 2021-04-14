#pragma once

#include <GL/glew.h>

namespace GL {

// Bindable
enum TextureTargetT1D    : GLenum { TEXTURE_1D = GL_TEXTURE_1D };
enum TextureTargetT2D    : GLenum { TEXTURE_2D = GL_TEXTURE_2D };
enum TextureTargetT3D    : GLenum { TEXTURE_3D = GL_TEXTURE_3D };
enum TextureTargetT1DA   : GLenum { TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY };
enum TextureTargetT2DA   : GLenum { TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY };
enum TextureTargetTR     : GLenum { TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE };
enum TextureTargetTCM    : GLenum { TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP };
enum TextureTargetTB     : GLenum { TEXTURE_BUFFER = GL_TEXTURE_BUFFER };
enum TextureTargetT2DM   : GLenum { TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE };
enum TextureTargetT2DMA  : GLenum { TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY };

// Proxy
enum TextureTargetPT1D   : GLenum { PROXY_TEXTURE_1D = GL_PROXY_TEXTURE_1D };
enum TextureTargetPT2D   : GLenum { PROXY_TEXTURE_2D = GL_PROXY_TEXTURE_2D };
enum TextureTargetPT3D   : GLenum { PROXY_TEXTURE_3D = GL_PROXY_TEXTURE_3D };
enum TextureTargetPT1DA  : GLenum { PROXY_TEXTURE_1D_ARRAY = GL_PROXY_TEXTURE_1D_ARRAY };
enum TextureTargetPT2DA  : GLenum { PROXY_TEXTURE_2D_ARRAY = GL_PROXY_TEXTURE_2D_ARRAY };
enum TextureTargetPTR    : GLenum { PROXY_TEXTURE_RECTANGLE = GL_PROXY_TEXTURE_RECTANGLE };
enum TextureTargetPTCM   : GLenum { PROXY_TEXTURE_CUBE_MAP = GL_PROXY_TEXTURE_CUBE_MAP };
enum TextureTargetPT2DM  : GLenum { PROXY_TEXTURE_2D_MULTISAMPLE = GL_PROXY_TEXTURE_2D_MULTISAMPLE };
enum TextureTargetPT2DMA : GLenum { PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY = GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY };

enum TextureTargetCubeMapFace : GLenum {
    TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    TEXTURE_CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    TEXTURE_CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    TEXTURE_CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    TEXTURE_CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    TEXTURE_CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

namespace TextureTarget {

class Base {
public:
    constexpr GLenum Get() const { return value; }
protected:
    constexpr Base(GLenum v) : value(v) {}
private:
    GLenum value;
};

class Mipmapable : public Base{
public:
    constexpr Mipmapable(TextureTargetT1D v)   : Base(v) {}
    constexpr Mipmapable(TextureTargetT2D v)   : Base(v) {}
    constexpr Mipmapable(TextureTargetT3D v)   : Base(v) {}
    constexpr Mipmapable(TextureTargetT1DA v)  : Base(v) {}
    constexpr Mipmapable(TextureTargetT2DA v)  : Base(v) {}
    constexpr Mipmapable(TextureTargetTCM v)   : Base(v) {}
};

class Bindable : public Base{
public:
    constexpr Bindable(Mipmapable v)         : Base(v) {}

    constexpr Bindable(TextureTargetTR v)    : Base(v) {}
    constexpr Bindable(TextureTargetTB v)    : Base(v) {}
    constexpr Bindable(TextureTargetT2DM v)  : Base(v) {}
    constexpr Bindable(TextureTargetT2DMA v) : Base(v) {}
};

class Parameter : public Base {
public:
    constexpr Parameter(TextureTargetT1D v)   : Base(v) {}
    constexpr Parameter(TextureTargetT2D v)   : Base(v) {}
    constexpr Parameter(TextureTargetT3D v)   : Base(v) {}
    constexpr Parameter(TextureTargetT1DA v)  : Base(v) {}
    constexpr Parameter(TextureTargetT2DA v)  : Base(v) {}
    constexpr Parameter(TextureTargetTR v)    : Base(v) {}
    constexpr Parameter(TextureTargetTCM v)   : Base(v) {}
};

class Proxy : public Base {
public:
    constexpr Proxy(TextureTargetPT1D v)     : Base(v) {}
    constexpr Proxy(TextureTargetPT2D v)     : Base(v) {}
    constexpr Proxy(TextureTargetPT3D v)     : Base(v) {}
    constexpr Proxy(TextureTargetPT1DA v)    : Base(v) {}
    constexpr Proxy(TextureTargetPT2DA v)    : Base(v) {}
    constexpr Proxy(TextureTargetPTR v)      : Base(v) {}
    constexpr Proxy(TextureTargetPTCM v)     : Base(v) {}
    constexpr Proxy(TextureTargetPT2DM v)    : Base(v) {}
    constexpr Proxy(TextureTargetPT2DMA v)   : Base(v) {}
};

class TwoD : public Base {
public:
    constexpr TwoD(TextureTargetT2D v)          : Base(v) {}
    constexpr TwoD(TextureTargetPT2D v)         : Base(v) {}
    constexpr TwoD(TextureTargetT1DA v)         : Base(v) {}
    constexpr TwoD(TextureTargetPT1DA v)        : Base(v) {}
    constexpr TwoD(TextureTargetCubeMapFace v)  : Base(v) {}
    constexpr TwoD(TextureTargetPTCM v)         : Base(v) {}
};

class Rectangle : public Base {
public:
    constexpr Rectangle(TextureTargetTR v)   : Base(v) {}
    constexpr Rectangle(TextureTargetPTR v)  : Base(v) {}
};

template<GLenum target>
struct ProxyTarget {
// static constexpr auto value =
};

template<>
struct ProxyTarget<TEXTURE_1D> {
    static constexpr auto value = PROXY_TEXTURE_1D;
};

template<>
struct ProxyTarget<TEXTURE_2D> {
    static constexpr auto value = PROXY_TEXTURE_2D;
};

template<>
struct ProxyTarget<TEXTURE_3D> {
    static constexpr auto value = PROXY_TEXTURE_3D;
};

template<>
struct ProxyTarget<TEXTURE_1D_ARRAY> {
    static constexpr auto value = PROXY_TEXTURE_1D_ARRAY;
};

template<>
struct ProxyTarget<TEXTURE_2D_ARRAY> {
    static constexpr auto value = PROXY_TEXTURE_2D_ARRAY;
};

template<>
struct ProxyTarget<TEXTURE_RECTANGLE> {
    static constexpr auto value = PROXY_TEXTURE_RECTANGLE;
};

template<>
struct ProxyTarget<TEXTURE_CUBE_MAP> {
    static constexpr auto value = PROXY_TEXTURE_CUBE_MAP;
};

template<>
struct ProxyTarget<TEXTURE_2D_MULTISAMPLE> {
    static constexpr auto value = PROXY_TEXTURE_2D_MULTISAMPLE;
};

template<>
struct ProxyTarget<TEXTURE_2D_MULTISAMPLE_ARRAY> {
    static constexpr auto value = PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY;
};

} // namespace TextureTarget
} // namespace GL