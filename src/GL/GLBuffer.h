#pragma once

#include "GLObject.h"

namespace GL {

template<class Target>
class Buffer : public ObjectArray<Buffer<Target> > {
public:
    typedef ObjectArray<Buffer<Target> > Base; 
    friend Base;
    using Base::Bind;
    using Base::BindDefault;

    static constexpr GLenum target = Target::value;

    static void Data(GLsizeiptr size, const GLvoid* data, GLenum usage);

private:
    static void DestroyArray(GLsizei n, const GLuint* handles);
    static void CreateArray(GLsizei n, GLuint* handles);
    static void BindImpl(GLuint handle);
};

struct ArrayBufferTarget {
    static constexpr GLenum value = GL_ARRAY_BUFFER;
};

using ArrayBuffer = Buffer<ArrayBufferTarget>;

} // namespace 

#include "GLBuffer.inl"