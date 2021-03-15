#pragma once

#include "GLObject.h"

namespace GL {

enum BufferUsage : GLenum {
    STREAM_DRAW = GL_STREAM_DRAW,
    STREAM_READ = GL_STREAM_READ,
    STREAM_COPY = GL_STREAM_COPY,
    STATIC_DRAW = GL_STATIC_DRAW,
    STATIC_READ = GL_STATIC_READ,
    STATIC_COPY = GL_STATIC_COPY,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    DYNAMIC_READ = GL_DYNAMIC_READ,
    DYNAMIC_COPY = GL_DYNAMIC_COPY
};

template<class Target>
class Buffer : public ObjectArray<Buffer<Target> > {
public:
    typedef ObjectArray<Buffer<Target> > Base; 
    friend Base;
    using Base::Bind;
    using Base::BindDefault;

    static constexpr GLenum target = Target::value;

    static void Data(GLsizeiptr size, const GLvoid* data, BufferUsage usage)    { glBufferData(target, size, data, usage); }
    template<class T, std::size_t N>
    static void Data(const T (& data)[N], BufferUsage usage)                    { Data(sizeof(data), data, usage); }

    static void SubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)   { glBufferSubData(target, offset, size, data); }
    template<class T, std::size_t N>
    static void SubData(GLintptr offset, const T (& data)[N])                   { SubData(offset, sizeof(data), data); }
    template<class T, std::size_t N>
    static void SubData(const T (& data)[N])                                    { SubData(0, sizeof(data), data); }
private:
    static void DestroyArray(GLsizei n, const GLuint* handles)                  { glDeleteBuffers(n, handles); }
    static void CreateArray(GLsizei n, GLuint* handles)                         { glGenBuffers(n, handles); }
    static void BindImpl(GLuint handle)                                         { glBindBuffer(target, handle); }
};

struct ArrayBufferTarget {
    static constexpr GLenum value = GL_ARRAY_BUFFER;
};

using ArrayBuffer = Buffer<ArrayBufferTarget>;

} // namespace GL
