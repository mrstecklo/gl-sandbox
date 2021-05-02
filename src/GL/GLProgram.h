#pragma once

#include "GLObject.h"
#include "GLUniform.h"
#include <string>

namespace GL {

enum UniformLocation : GLint;

class Shader;

class Program : public Object<Program> {
public:
    typedef Object<Program> Base;
    friend Base;

    Program() :
        Base(glCreateProgram()) {}

    explicit constexpr Program(std::nullptr_t p) :
        Base(p) {}

    inline static Program Create(const GLchar* vertex, const GLchar* geometry, const GLchar* fragment);

    void Use() const { glUseProgram(Get()); }

    inline void Attach(const Shader& s);
    inline void Detach(const Shader& s);

    void Link() { glLinkProgram(Get()); }

    Uniform GetUniformLocation(const GLchar* name) { return Uniform(glGetUniformLocation(Get(), name)); }

    inline std::string GetInfoLog() const;
    inline GLboolean GetLinkStatus() const;

private:
    static void Destroy(GLuint handle) { glDeleteProgram(handle); }
};

} // namespace GL
