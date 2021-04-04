#pragma once

#include <stdexcept>

namespace GL {

void Program::Attach(const Shader& s) { glAttachShader(Get(), s.Get()); }
void Program::Detach(const Shader& s) { glDetachShader(Get(), s.Get()); }

Program Program::Create(const GLchar* vertex, const GLchar* geometry, const GLchar* fragment)
{
    Program prog;

    Shader vertexSh(nullptr);           
    Shader geometrySh(nullptr);
    Shader fragmentSh(nullptr);

    if(vertex) {
        vertexSh = Shader::Create(VERTEX_SHADER, vertex);
        prog.Attach(vertexSh);
    }

    if(geometry) {
        geometrySh = Shader::Create(GEOMETRY_SHADER, geometry);
        prog.Attach(geometrySh);
    }

    if(fragment) {
        fragmentSh = Shader::Create(FRAGMENT_SHADER, fragment);
        prog.Attach(fragmentSh);
    }

    prog.Link();
    if(!prog.GetLinkStatus()) {
        // shaders will be detached by OpenGL automatically
        // after ~Program() and ~Shader() are invoked
        throw std::runtime_error(prog.GetInfoLog());
    }

    if(vertex) {
        prog.Detach(vertexSh);
    }

    if(geometry) {
        prog.Detach(geometrySh);
    }

    if(fragment) {
       prog. Detach(fragmentSh);
    }

    return prog;
}

GLboolean Program::GetLinkStatus() const
{
    GLint result = GL_FALSE;
    glGetProgramiv(Get(), GL_LINK_STATUS, &result);
    return result;
}

std::string Program::GetInfoLog() const
{
    GLint length = 0;
    glGetProgramiv(Get(), GL_INFO_LOG_LENGTH, &length);
    if ( length > 0 ) {
        std::string message(length, '\0');
        glGetProgramInfoLog(Get(),length, nullptr, &message[0]);
        return message;
    } else {
        return std::string();
    }
}

} // namespace GL