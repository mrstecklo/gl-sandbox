#pragma once

#include <GL/glew.h>

namespace GL {
namespace Debug {

enum class Source : GLenum {
    API             = GL_DEBUG_SOURCE_API,
    WINDOW_SYSTEM   = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
    SHADER_COMPILER = GL_DEBUG_SOURCE_SHADER_COMPILER,
    THIRD_PARTY     = GL_DEBUG_SOURCE_THIRD_PARTY,
    APPLICATION     = GL_DEBUG_SOURCE_APPLICATION,
    OTHER           = GL_DEBUG_SOURCE_OTHER
};

enum class Type : GLenum {
    ERROR = GL_DEBUG_TYPE_ERROR,
    DEPRECATED_BEHAVIOR = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
    UNDEFINED_BEHAVIOR = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
    PORTABILITY = GL_DEBUG_TYPE_PORTABILITY,
    PERFORMANCE = GL_DEBUG_TYPE_PERFORMANCE,
    MARKER = GL_DEBUG_TYPE_MARKER,
    PUSH_GROUP = GL_DEBUG_TYPE_PUSH_GROUP,
    POP_GROUP = GL_DEBUG_TYPE_POP_GROUP,
    OTHER = GL_DEBUG_TYPE_OTHER
};

enum class Severity : GLenum {
    HIGH = GL_DEBUG_SEVERITY_HIGH,
    MEDIUM = GL_DEBUG_SEVERITY_MEDIUM,
    LOW = GL_DEBUG_SEVERITY_LOW,
    NOTIFICATION = GL_DEBUG_SEVERITY_NOTIFICATION
};

typedef void (GLAPIENTRY *Callback) (
    Source,
    Type,
    GLuint,         // id
    Severity,
    GLsizei,        // message size
    const GLchar*,  // message
    const void*);   // userParam

inline void Enable()  { glEnable(GL_DEBUG_OUTPUT); }
inline void Disable() { glDisable(GL_DEBUG_OUTPUT); }
inline void RegisterCallback(Callback cb, void* userParam) { glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(cb), userParam); }

} // namespace Debug
} // namespace GL

const char* ToString(GL::Debug::Severity s);
const char* ToString(GL::Debug::Type t);
const char* ToString(GL::Debug::Source s);
