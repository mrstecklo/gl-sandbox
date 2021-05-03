#pragma once

#include "GLCPP/Program.h"
#include "GLCPP/VertexAttribArray.h"

namespace GUI {

class Program {
public:
    static constexpr auto attrVertices = GL::VertexAttrib(0);
    static constexpr auto attrUV = GL::VertexAttrib(1);

    Program(GL::Program&& p, bool flipTexture) :
        impl(std::move(p)),
        MVP(GL::UniformMatrix4f(impl.GetUniformLocation("MVP")))
    {
        if(flipTexture) {
            GL::UniformMatrix3x2f UVMat(impl.GetUniformLocation("UVMat"));
            static const float ivertedUVMat[3][2] = {
                {1.f,  0.f},
                {0.f, -1.f},
                {0.f,  1.f}
            };
            impl.Use();
            UVMat.Set(ivertedUVMat);
        }
    }

    void Use() const { impl.Use(); }
    GL::UniformMatrix4f& GetMVP() { return MVP; }

private:

    GL::Program impl {nullptr};
    GL::UniformMatrix4f MVP{0};
};

} // namespace GUI
