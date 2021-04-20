#pragma once

namespace GL {

Shader Shader::Create(ShaderType type, const GLchar* string)
{
    Shader sh(type);
    sh.Source(string);
    sh.Compile();
    if(!sh.GetCompileStatus()) {
        throw std::runtime_error(sh.GetInfoLog());
    }

    return sh;
}

GLboolean Shader::GetCompileStatus() const
{
    GLint result = GL_FALSE;
    glGetShaderiv(Get(), GL_COMPILE_STATUS, &result);
    return result;
}

std::string Shader::GetInfoLog() const
{
    GLint length = 0;
    glGetShaderiv(Get(), GL_INFO_LOG_LENGTH, &length);
    if ( length > 0 ) {
        std::string message(length, '\0');
        glGetShaderInfoLog(Get(),length, nullptr, &message[0]);
        return message;
    } else {
        return std::string();
    }
}


} // namespace GL
