#pragma once

#include <GL/glew.h>

namespace GL {

enum PixelLayout : GLenum {
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    BGR = GL_BGR,
    RGBA = GL_RGBA,
    BGRA = GL_BGRA
};

enum IntegralType : GLenum {
    BYTE            = GL_BYTE,
    UNSIGNED_BYTE   = GL_UNSIGNED_BYTE,
    SHORT           = GL_SHORT,
    UNSIGNED_SHORT  = GL_UNSIGNED_SHORT,
    INT             = GL_INT,
    UNSIGNED_INT    = GL_UNSIGNED_INT
};

enum HalfFloatingType   : GLenum { HALF_FLOAT = GL_HALF_FLOAT };
enum SingleFloatingType : GLenum { FLOAT      = GL_FLOAT };
enum DoubleFloatingType : GLenum { DOUBLE     = GL_DOUBLE };

enum PackedPixelType : GLenum {
    UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
    UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
    UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
    UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
    UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
    UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2
};

enum ReversePackedPixelType : GLenum {
    UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
    UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
    UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
};

class PixelTypeBase {
public:
    constexpr GLenum Get() const { return value; }
protected:
    constexpr PixelTypeBase(GLenum v) : value(v) {}
private:
    GLenum value;
};

class FundamentalType : public PixelTypeBase {
public:
    constexpr FundamentalType(IntegralType v)       : PixelTypeBase(v) {}
    constexpr FundamentalType(SingleFloatingType v) : PixelTypeBase(v) {}
};

class FloatingType : public PixelTypeBase {
public:
    constexpr FloatingType(HalfFloatingType v)   : PixelTypeBase(v) {}
    constexpr FloatingType(SingleFloatingType v) : PixelTypeBase(v) {}
    constexpr FloatingType(DoubleFloatingType v) : PixelTypeBase(v) {}
};

class BitFieldType : public PixelTypeBase {
    constexpr BitFieldType(PackedPixelType v)           : PixelTypeBase(v) {}
    constexpr BitFieldType(ReversePackedPixelType v)    : PixelTypeBase(v) {}
};

class PixelType : public PixelTypeBase {
public:
    constexpr PixelType(FundamentalType v) : PixelTypeBase(v.Get()) {}
    constexpr PixelType(BitFieldType v) : PixelTypeBase(v.Get()) {}
};

template<class T>
struct TypeTraits {
    // static constexpr auto type = 
};

template<>
struct TypeTraits<GLbyte> {
    static constexpr auto type = BYTE;
    static constexpr auto layout = RED;
};

template<>
struct TypeTraits<GLubyte> {
    static constexpr auto type = UNSIGNED_BYTE;
    static constexpr auto layout = RED;
};

template<>
struct TypeTraits<GLshort> {
    static constexpr auto type = SHORT;
    static constexpr auto layout = RED;
};

template<>
struct TypeTraits<GLushort> {
    static constexpr auto type = UNSIGNED_SHORT;
    static constexpr auto layout = RED;
};

template<>
struct TypeTraits<GLint> {
    static constexpr auto type = INT;
    static constexpr auto layout = RED;
};

template<>
struct TypeTraits<GLuint> {
    static constexpr auto type = UNSIGNED_INT;
    static constexpr auto layout = RED;
};

template<>
struct TypeTraits<GLfloat> {
    static constexpr auto type = FLOAT;
    static constexpr auto layout = RED;
};

namespace Pixel {

template<class T>
struct RG {
    T red;
    T green;
};

template<class T>
struct RGB {
    T red;
    T green;
    T blue;
};

template<class T>
struct BGR {
    T blue;
    T green;
    T red;
};

template<class T>
struct RGBA {
    T red;
    T green;
    T blue;
    T alpha;
};

template<class T>
struct BGRA {
    T blue;
    T green;
    T red;
    T alpha;
};

} // namespace Pixel

template<class T>
struct TypeTraits<Pixel::RG<T> > {
    static constexpr auto type = TypeTraits<T>::type;
    static constexpr auto layout = RG;
};

template<class T>
struct TypeTraits<Pixel::RGB<T> > {
    static constexpr auto type = TypeTraits<T>::type;
    static constexpr auto layout = RGB;
};

template<class T>
struct TypeTraits<Pixel::BGR<T> > {
    static constexpr auto type = TypeTraits<T>::type;
    static constexpr auto layout = BGR;
};

template<class T>
struct TypeTraits<Pixel::RGBA<T> > {
    static constexpr auto type = TypeTraits<T>::type;
    static constexpr auto layout = RGBA;
};

template<class T>
struct TypeTraits<Pixel::BGRA<T> > {
    static constexpr auto type = TypeTraits<T>::type;
    static constexpr auto layout = BGRA;
};

} // namespace GL