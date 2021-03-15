#pragma once

#include "GLObject.h"

namespace GL {

class VertexArray : public ObjectArray<VertexArray> {
public:
    typedef ObjectArray<VertexArray> Base; 
    friend Base;
    using Base::Bind;
    using Base::BindDefault;

private:
    static void DestroyArray(GLsizei n, const GLuint* handles)  { glDeleteVertexArrays(n, handles); }
    static void CreateArray(GLsizei n, GLuint* handles)         { glGenVertexArrays(n, handles); }
    static void BindImpl(GLuint handle)                         { glBindVertexArray(handle); }
};

} // namespace GL