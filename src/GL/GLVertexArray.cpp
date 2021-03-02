#include "GLVertexArray.h"

namespace GL {

void VertexArray::BindImpl(GLuint handle)
{
    glBindVertexArray(handle);
}

void VertexArray::DestroyArray(GLsizei n, const GLuint* handles)
{
    glDeleteVertexArrays(n, handles);
}

void VertexArray::CreateArray(GLsizei n, GLuint* handles)
{
    glGenVertexArrays(n, handles);
}

} // namespace GL