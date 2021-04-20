#pragma once

#include "GLTextureTarget.h"
#include "GLFormat.h"
#include "GLType.h"
#include "GLTextureFiltration.h"
#include "GLObject.h"

namespace GL {

template<class T>
void TexImage2D(
    TextureTarget::TwoD target,
 	GLint level,
 	Format internalFormat,
 	GLsizei width,
 	GLsizei height,
 	const T* data)
{
    glTexImage2D(target.Get(), level, internalFormat.Get(), width, height, 0, TypeTraits<T>::layout, TypeTraits<T>::type, data);
}

template<class T>
void TexImage2D(
    TextureTarget::Rectangle target,
 	Format internalFormat,
 	GLsizei width,
 	GLsizei height,
 	const T* data)
{
    glTexImage2D(target.Get(), 0, internalFormat.Get(), width, height, 0, TypeTraits<T>::layout, TypeTraits<T>::type, data);
}

template<class Target>
class Texture : public ObjectArray<Texture<Target> > {
public:
    typedef ObjectArray<Texture<Target> > Base; 
    friend Base;

    static constexpr auto target = Target::value;
    template<class T>
    using Proxy = TextureTarget::ProxyTarget<T::value>;

    Texture() = default;
    constexpr explicit Texture(std::nullptr_t p) :
        Base(p) {}

    template<class T>
    static void Image2D(GLint level, Format internalFormat, GLsizei width, GLsizei height, const T* data)
        { TexImage2D(TextureTarget::TwoD(target), level, internalFormat, width, height, data); }

    template<class T>
    static void Image2D(Format internalFormat, GLsizei width, GLsizei height, const T* data)
        { TexImage2D(TextureTarget::Rectangle(target), internalFormat, width, height, data); }

    template<class T>
    static void Image2D(TextureTargetCubeMapFace tgt, GLint level, Format internalFormat, GLsizei width, GLsizei height, const T* data)
    { 
        static_assert(target == TEXTURE_CUBE_MAP, "Cube map face target is only available for cube map texures");
        TexImage2D(tgt, level, internalFormat, width, height, data);
    }

    template<class T>
    static void ProxyImage2D(GLint level, Format internalFormat, GLsizei width, GLsizei height)
        { TexImage2D<T>(TextureTarget::TwoD(Proxy<Target>::value), level, internalFormat, width, height, nullptr); }

    template<class T>
    static void ProxyImage2D(Format internalFormat, GLsizei width, GLsizei height)
        { TexImage2D<T>(TextureTarget::Rectangle(Proxy<Target>::value), internalFormat, width, height, nullptr); }

    static void CompressedImage2D(GLint level, CompressedFormat internalformat, GLsizei width, GLsizei height, GLsizei imageSize, const GLvoid *	data)
        { glCompressedTexImage2D(TextureTarget::TwoD(target).Get(), level, internalformat.Get(), width, height, 0, imageSize, data); }

    static void ProxyCompressedImage2D(GLint level, CompressedFormat internalformat, GLsizei width, GLsizei height, GLsizei imageSize)
        { glCompressedTexImage2D(TextureTarget::TwoD(Proxy<Target>::value).Get(), level, internalformat.Get(), width, height, 0, imageSize, nullptr); }

    static void SetMinFilter(TextureMinFilter f) { SetParameter(GL_TEXTURE_MIN_FILTER, f.Get()); }
    static void SetMagFilter(TextureMagFilter f) { SetParameter(GL_TEXTURE_MAG_FILTER, f.Get()); }

    static void GenerateMipmap() { glGenerateMipmap(TextureTarget::Mipmapable(target).Get()); }

private:
    static void DestroyArray(GLsizei n, const GLuint* handles)  { glDeleteTextures(n, handles); }
    static void CreateArray(GLsizei n, GLuint* handles)         { glGenTextures(n, handles); }
    static void BindImpl(GLuint handle)                         { glBindTexture(TextureTarget::Bindable(target).Get(), handle); }

    static void SetParameter(GLenum pname, GLint param)         { glTexParameteri(TextureTarget::Parameter(target).Get(), pname, param);}
};

struct Texture2DTarget {
    static constexpr auto value = TEXTURE_2D;
};

struct TextureCubeMapTarget {
    static constexpr auto value = TEXTURE_CUBE_MAP;
};

using Texture2D = Texture<Texture2DTarget>;
using TextureCubeMap = Texture<TextureCubeMapTarget>;

} // namespace GL
