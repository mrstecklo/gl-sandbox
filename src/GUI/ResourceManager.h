#pragma once

#include "ResourceManagerFwd.h"
#include "Model.h"

namespace GUI {

class ResourceManager {
public:

    Model CreateModel(
        int vertices,
        int texture,
        int program
    )
    {
        return CreateModelImpl(vertices, texture, program);
    }

    virtual ~ResourceManager() = default;

protected:

    Model NewModel(
        const std::shared_ptr<ArrayBuffer>& vertexBuffer,
        const std::shared_ptr<ArrayBuffer>& UVBuffer,
        const std::shared_ptr<GL::Texture2D>& texture,
        const std::shared_ptr<Program>& program
    )
    {
        return Model(vertexBuffer, UVBuffer, texture, program);
    }
    
private:

    virtual Model CreateModelImpl(
        int vertices,
        int texture,
        int program
    ) = 0;

};

} // namespace GUI
