#pragma once 

#include "Type.h"
#include "Util/Noncopyable.h"

namespace GL {

enum VertexAttrib : GLuint;

class VertexAttribArray : Util::Noncopyable {
public:
    constexpr VertexAttribArray() = default;
    VertexAttribArray(VertexAttrib i) :
        valid(true),
        index(i)
    {
        glEnableVertexAttribArray(index);
    }

    ~VertexAttribArray() { if(valid) { glDisableVertexAttribArray(index); } }

    VertexAttribArray(VertexAttribArray&& other) :
        valid(std::exchange(other.valid, false)),
        index(other.index)
    {}  
    
    VertexAttribArray& operator=(VertexAttribArray&& other)
    {
        std::swap(valid, other.valid);
        std::swap(index, other.index);
        return *this;
    }
private:
    bool valid = false;
    GLuint index = 0;
};

inline void VertexAttribPointer(
    VertexAttrib    index,
    GLint  	        size,
    IntegralType  	type,
    GLboolean  	    normalized,
    GLsizei  	    stride,
    const GLvoid*  	pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

inline void VertexAttribPointer(
    VertexAttrib    index,
    GLint  	        size,
    FloatingType  	type,
    GLsizei  	    stride,
    const GLvoid*  	pointer)
{
    glVertexAttribPointer(index, size, type.Get(), GL_FALSE, stride, pointer);
}

inline void VertexAttribPointer(
    VertexAttrib    index,
    GLint  	        size,
    FloatingType  	type,
    GLboolean  	    /*normalized*/,
    GLsizei  	    stride,
    const GLvoid*  	pointer)
{
    VertexAttribPointer(index, size, type, stride, pointer);
}

inline void VertexAttribPointer(
    VertexAttrib    index,
    GLint  	        size,
    VertexAttribPackedType type,
    GLsizei  	    stride,
    const GLvoid*  	pointer)
{
    glVertexAttribPointer(index, size, type.Get(), GL_FALSE, stride, pointer);
}

inline void VertexAttribPointer(
    VertexAttrib    index,
    GLint  	        size,
    VertexAttribPackedType type,
    GLboolean  	    /*normalized*/,
    GLsizei  	    stride,
    const GLvoid*  	pointer)
{
    VertexAttribPointer(index, size, type, stride, pointer);
}

inline void VertexAttribIPointer(
    VertexAttrib    index,
 	GLint  	        size,
 	IntegralType    type,
 	GLsizei  	    stride,
 	const GLvoid*  	pointer)
{
    glVertexAttribIPointer(index, size, type, stride, pointer);
}

} // namespace GL
