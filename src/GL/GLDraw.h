#pragma once

#include <GL/glew.h>

namespace GL {

enum Comparsion : GLenum {
    NEVER = GL_NEVER,
    LESS = GL_LESS,
    EQUAL = GL_EQUAL,
    LEQUAL = GL_LEQUAL,
    GREATER = GL_GREATER,
    NOTEQUAL = GL_NOTEQUAL,
    GEQUAL = GL_GEQUAL,
    ALWAYS = GL_ALWAYS
};

inline void EnableDepthTest() { glEnable(GL_DEPTH_TEST); }
inline void DisableDepthTest() { glDisable(GL_DEPTH_TEST); }
inline void DepthFunc(Comparsion func) { glDepthFunc(func); }

inline void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) { glClearColor(red, green, blue, alpha); }

enum ClearMask : GLbitfield {
    COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
    DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
    STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT
};

inline void Clear(ClearMask mask) { glClear(mask); }
inline void Clear(ClearMask first, ClearMask second) { glClear(static_cast<GLbitfield>(first) | static_cast<GLbitfield>(second)); }
inline void Clear(ClearMask first, ClearMask second, ClearMask third) { glClear(static_cast<GLbitfield>(first) | static_cast<GLbitfield>(second) | static_cast<GLbitfield>(third)); }

enum DrawArraysMode : GLenum {
    POINTS = GL_POINTS,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_LOOP = GL_LINE_LOOP,
    LINES = GL_LINES,
    LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
    LINES_ADJACENCY = GL_LINES_ADJACENCY,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY
};

inline void DrawArrays(DrawArraysMode mode, GLint first, GLsizei count) { glDrawArrays(mode, first, count); }
inline void DrawArrays(DrawArraysMode mode, GLsizei count)              { DrawArrays(mode, 0, count); }

} // namespace GL