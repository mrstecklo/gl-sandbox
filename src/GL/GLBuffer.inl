#pragma once

namespace GL {

template<class Target>
void Buffer<Target>::Data(GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    glBufferData(target, size, data, usage);
}

template<class Target>
void Buffer<Target>::BindImpl(GLuint handle)
{
    glBindBuffer(target, handle);
}

template<class Target>
void Buffer<Target>::DestroyArray(GLsizei n, const GLuint* handles)
{
    glDeleteBuffers(n, handles);
}

template<class Target>
void Buffer<Target>::CreateArray(GLsizei n, GLuint* handles)
{
    glGenBuffers(n, handles);
}

} // namespace GL