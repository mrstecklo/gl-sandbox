#pragma once

#include "GUI/ResourceManager.h"

namespace Tetris {

namespace Resource {

enum Vertices : int {
    V_INVALID,
    V_CUBE,
    V_TRIANGLE,

    V_NUM_RESOURCES
};

enum Texture : int {
    T_INVALID,
    T_TEMPLATE,

    T_NUM_RESOURCES
};

enum Program : int {
    P_INVALID,
    P_GENERIC,

    P_NUM_RESOURCES
};

} // namespace Resource

class ResourceManager : public GUI::ResourceManager {
public:

    template<class T>
    using Container = std::vector<std::weak_ptr<T> >;

private:

    GUI::Model CreateModelImpl(
        int vertices,
        int texture,
        int program
    ) override;

    std::shared_ptr<GUI::ArrayBuffer>   GetVertexBuffer(int idx);
    std::shared_ptr<GUI::ArrayBuffer>   GetUVBuffer(int idx);
    std::shared_ptr<GL::Texture2D>      GetTexture(int idx);
    std::shared_ptr<GUI::Program>       GetProgram(int idx);

    static std::shared_ptr<GUI::ArrayBuffer>   CreateVertexBuffer(int idx);
    static std::shared_ptr<GUI::ArrayBuffer>   CreateUVBuffer(int idx);
    static std::shared_ptr<GL::Texture2D>      CreateTexture(int idx);
    static std::shared_ptr<GUI::Program>       CreateProgram(int idx);

    static GL::Texture2D LoadTexture(int idx, uint32_t* mipmapCount);

    Container<GUI::ArrayBuffer> vertexContainer{Resource::V_NUM_RESOURCES - 1};
    Container<GUI::ArrayBuffer> UVContainer{Resource::V_NUM_RESOURCES - 1};
    Container<GL::Texture2D>    textureContainer{Resource::T_NUM_RESOURCES - 1};
    Container<GUI::Program>     programContainer{Resource::P_NUM_RESOURCES - 1};

    static constexpr GLint vertexSize = 3;
    static constexpr GLint UVSize = 2;
    static constexpr GLsizei numVertices = 36;

    static const GLchar vertexShader[];
    static const GLchar fragmentShader[];

    static const GLfloat g_vertex_buffer_data[numVertices][vertexSize];
    static const GLfloat g_uv_buffer_data[numVertices][UVSize];
};

} // namespace Tetris
