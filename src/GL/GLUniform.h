#pragma once

#include <GL/glew.h>

namespace GL {

class Uniform {
public:
    constexpr explicit Uniform(GLint loc) : location(loc) {}
    constexpr GLint Get() const { return location; }

private:
    GLint location;
};

template<class Size>
class UniformMatrix : public Uniform {
public:
    constexpr static auto cols = Size::cols;
    constexpr static auto rows = Size::rows;

    constexpr explicit UniformMatrix(GLint loc) : Uniform(loc) {}
    constexpr explicit UniformMatrix(Uniform u) : Uniform(u) {}

    void Set(GLsizei count, GLboolean transpose, const GLfloat* value)      { Size::Set(Get(), count, transpose, value); }
    void Set(GLsizei count, const GLfloat (&value)[cols][rows])             { Set(count, false, &value[0][0]); }
    void SetTransposed(GLsizei count, const GLfloat (&value)[rows][cols])   { Set(count, true, &value[0][0]); }

    void Set(GLboolean transpose, const GLfloat* value)                     { Set(1, transpose, value); }
    void Set(const GLfloat (&value)[cols][rows])                            { Set(1, value); }
    void SetTransposed(const GLfloat (&value)[rows][cols])                  { SetTransposed(1, value); }

    void Set(const GLfloat* value)                                          { Set(false, value); }
};

struct UniformMatrixSize4x4 {
    constexpr static std::size_t cols = 4;
    constexpr static std::size_t rows = 4;

    static void Set(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4fv(location, count, transpose, value); }
};

using UniformMatrix4f = UniformMatrix<UniformMatrixSize4x4>;

} // namespace GL