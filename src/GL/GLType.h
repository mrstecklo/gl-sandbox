#pragma once

#include <GL/glew.h>

namespace GL {

enum IntegralType : GLenum {
    BYTE            = GL_BYTE,
    UNSIGNED_BYTE   = GL_UNSIGNED_BYTE,
    SHORT           = GL_SHORT,
    UNSIGNED_SHORT  = GL_UNSIGNED_SHORT,
    INT             = GL_INT,
    UNSIGNED_INT    = GL_UNSIGNED_INT
};

enum FloatingType : GLenum {
     HALF_FLOAT = GL_HALF_FLOAT,
     FLOAT      = GL_FLOAT,
     DOUBLE     = GL_DOUBLE
};

enum BitFieldType : GLenum {
    INT_2_10_10_10_REV          = GL_INT_2_10_10_10_REV,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
};

class Type {
public:
    constexpr Type(IntegralType v) : value(v) {}
    constexpr Type(FloatingType v) : value(v) {}
    constexpr Type(BitFieldType v) : value(v) {}

    constexpr GLenum get() const { return value; }
private:
    GLenum value;
};

} // namespace GL