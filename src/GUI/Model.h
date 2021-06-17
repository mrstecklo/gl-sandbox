#pragma once

#include "GLCPP/Texture.h"
#include "Object.h"
#include "Program.h"
#include "ArrayBuffer.h"
#include "ResourceManagerFwd.h"

namespace GUI {

class Model : public Object {
public:
    friend class ResourceManager;

    Model() = default;


    const ArrayBuffer*      GetVertexBuffer()   const { return vertex.get(); }
    const ArrayBuffer*      GetUVBuffer()       const { return UV.get(); }
    const GL::Texture2D*    GetTexture()        const { return tex.get(); }
    Program*                GetProgram()        const { return p.get(); }

    bool Valid() const { return vertex && UV && tex && p; }

private:

    Model(
        const std::shared_ptr<ArrayBuffer>& vertexBuffer,
        const std::shared_ptr<ArrayBuffer>& UVBuffer,
        const std::shared_ptr<GL::Texture2D>& texture,
        const std::shared_ptr<Program>& program) :
            vertex(vertexBuffer),
            UV(UVBuffer),
            tex(texture),
            p(program)
    {
    }

    std::shared_ptr<ArrayBuffer>    vertex;
    std::shared_ptr<ArrayBuffer>    UV;
    std::shared_ptr<GL::Texture2D>  tex;
    std::shared_ptr<Program>        p;
};

} // namespace GUI
