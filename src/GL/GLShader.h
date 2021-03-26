#pragma once

#include "GLObject.h"
#include "GLProgram.h"

namespace GL {

enum ShaderType : GLenum {
    VERTEX_SHADER   = GL_VERTEX_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

class Shader : public Object<Shader> {
public:
    typedef Object<Shader> Base;
    friend Base;
    friend void Program::Attach(const Shader&);
    friend void Program::Detach(const Shader&);

    constexpr explicit Shader(std::nullptr_t p) :
        Base(p) {}

    explicit Shader(ShaderType type) :
        Base(glCreateShader(type)) {}

    static Shader Create(ShaderType type, const GLchar* string);

    void Source(GLsizei count, const GLchar **string, const GLint *length = nullptr)    { glShaderSource(Get(), count, string, length); }

    template<GLsizei count>
    void Source(const GLchar* (&string)[count], const GLint (&length)[count])           { Source(count, string, length); }

    template<GLsizei count>
    void Source(const GLchar* (&string)[count])                                         { Source(count, string, nullptr); }

    void Source(const GLchar* string, GLint length)                                     { Source(1, &string, &length); }
    void Source(const GLchar* string)                                                   { Source(1, &string, nullptr); }

    void Compile() { glCompileShader(Get()); }

    std::string GetInfoLog() const;
    GLboolean GetCompileStatus() const;

private:
    static void Destroy(GLuint handle) { glDeleteShader(handle); }
};

} // namespace GL

#include "GLProgram.inl"
#include "GLShader.inl"