#pragma once

#include "GLCPP.h"
#include "Object.h"
#include "Program.h"
#include "ArrayBuffer.h"
#include "ResourceManager.h"

namespace GUI {

class Model : public Object {
public:
    void Init(ArrayBufferFloat&& vertexBuffer, ArrayBufferFloat&& UVBuffer, GL::Texture2D&& texture, Program* p)
    {
        vertex = std::move(vertexBuffer);
        UV = std::move(UVBuffer);
        tex = std::move(texture);
        program = p;
    }

    const ArrayBuffer*      GetVertexBuffer()   const { return &vertex; }
    const ArrayBuffer*      GetUVBuffer()       const { return &UV; }
    const GL::Texture2D*    GetTexture()        const { return &tex; }
    Program*          GetProgram()        const { return program; }

private:

    ArrayBufferFloat vertex;
    ArrayBufferFloat UV;
    GL::Texture2D   tex {nullptr};
    Program*        program = nullptr;
};

} // namespace GUI
