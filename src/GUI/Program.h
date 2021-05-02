#pragma once

#include "GLCPP.h"

namespace GUI {

class Program {
public:
    static constexpr auto attrVertices = GL::VertexAttrib(0);
    static constexpr auto attrUV = GL::VertexAttrib(1);

    void Init(GL::Program&& p)
    {
        impl = std::move(p);
        MVP = GL::UniformMatrix4f(impl.GetUniformLocation("MVP"));
    }

    void Use() const { impl.Use(); }
    GL::UniformMatrix4f& GetMVP() { return MVP; }

private:

    GL::Program impl {nullptr};
    GL::UniformMatrix4f MVP{0};
};

} // namespace GUI
