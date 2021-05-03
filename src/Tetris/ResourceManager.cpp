#include "ResourceManager.h"
#include "Image/Load.h"
#include <sstream>

namespace Tetris {

const GLchar ResourceManager::vertexShader[] =
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;"
"layout(location = 1) in vec2 vertexUV;"
"out vec2 UV;"
"uniform mat4 MVP = {"
"{1.f, 0.f, 0.f, 0.f},"
"{0.f, 1.f, 0.f, 0.f},"
"{0.f, 0.f, 1.f, 0.f},"
"{0.f, 0.f, 0.f, 1.f}};"
"uniform mat3x2 UVMat = {"
"{1.f, 0.f},"
"{0.f, 1.f},"
"{0.f, 0.f}};"
"void main(){"
" gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
" UV = UVMat * vec3(vertexUV, 1);"
"}";

const GLchar ResourceManager::fragmentShader[] = 
"#version 330 core\n"
"in vec2 UV;"
"out vec3 color;"
"uniform sampler2D texSampler;"
"void main() {"
"color = texture( texSampler, UV ).rgb;;"
"}";

const GLfloat ResourceManager::g_vertex_buffer_data[numVertices][vertexSize] = { 
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

const GLfloat ResourceManager::g_uv_buffer_data[numVertices][UVSize] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};

GUI::Model ResourceManager::CreateModelImpl(
    int vertices,
    int texture,
    int program
)
{
    return NewModel(
        GetVertexBuffer(vertices),
        GetUVBuffer(vertices),
        GetTexture(texture),
        GetProgram(program)
    );
}

std::shared_ptr<GUI::ArrayBuffer> ResourceManager::GetVertexBuffer(int idx)
{
    if(idx != 0 && idx < Resource::V_NUM_RESOURCES) {
        auto& buffer = vertexContainer[idx - 1];
        if(buffer.expired()) {
            auto result = CreateVertexBuffer(idx);
            buffer = result;
            return result;
        } else {
            return std::shared_ptr<GUI::ArrayBuffer>(buffer);
        }
    } else {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Ivalid vertex resource ID: " << idx;
        throw std::runtime_error(ss.str());
    }
}

std::shared_ptr<GUI::ArrayBuffer>   ResourceManager::GetUVBuffer(int idx)
{
    if(idx != 0 && idx < Resource::V_NUM_RESOURCES) {
        auto& buffer = UVContainer[idx - 1];
        if(buffer.expired()) {
            auto result = CreateUVBuffer(idx);
            buffer = result;
            return result;
        } else {
            return std::shared_ptr<GUI::ArrayBuffer>(buffer);
        }
    } else {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Ivalid UV resource ID: " << idx;
        throw std::runtime_error(ss.str());
    }
}

std::shared_ptr<GL::Texture2D> ResourceManager::GetTexture(int idx)
{
    if(idx != 0 && idx < Resource::T_NUM_RESOURCES) {
        auto& buffer = textureContainer[idx - 1];
        if(buffer.expired()) {
            auto result = CreateTexture(idx);
            buffer = result;
            return result;
        } else {
            return std::shared_ptr<GL::Texture2D>(buffer);
        }
    } else {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Ivalid texture resource ID: " << idx;
        throw std::runtime_error(ss.str());
    }
}

std::shared_ptr<GUI::Program> ResourceManager::GetProgram(int idx)
{
    if(idx != 0 && idx < Resource::P_NUM_RESOURCES) {
        auto& buffer = programContainer[idx - 1];
        if(buffer.expired()) {
            auto result = CreateProgram(idx);
            buffer = result;
            return result;
        } else {
            return std::shared_ptr<GUI::Program>(buffer);
        }
    } else {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Ivalid shader resource ID: " << idx;
        throw std::runtime_error(ss.str());
    }
}

std::shared_ptr<GUI::ArrayBuffer>   ResourceManager::CreateVertexBuffer(int idx)
{
    using namespace Resource;
    switch(idx) {
    case V_CUBE:
        return std::make_shared<GUI::ArrayBufferFloat>(g_vertex_buffer_data, GL::STATIC_DRAW);
    case V_TRIANGLE:
        return std::make_shared<GUI::ArrayBufferFloat>(3, 3, &g_vertex_buffer_data[0][0], GL::STATIC_DRAW);
    default:
    {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Vertex resource ID: " << idx << " is valid, but not implemented";
        throw std::runtime_error(ss.str());
    }
    }
}

std::shared_ptr<GUI::ArrayBuffer>   ResourceManager::CreateUVBuffer(int idx)
{
    using namespace Resource;
    switch(idx) {
    case V_CUBE:
        return std::make_shared<GUI::ArrayBufferFloat>(g_uv_buffer_data, GL::STATIC_DRAW);
    case V_TRIANGLE:
        return std::make_shared<GUI::ArrayBufferFloat>(3, 2, &g_uv_buffer_data[0][0], GL::STATIC_DRAW);
    default:
    {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. UV resource ID: " << idx << " is valid, but not implemented";
        throw std::runtime_error(ss.str());
    }
    }
}

std::shared_ptr<GL::Texture2D> ResourceManager::CreateTexture(int idx)
{
    uint32_t mipmapCount = 1;
    auto result = std::make_shared<GL::Texture2D>(LoadTexture(idx, &mipmapCount));

    GL::Texture2D::SetMinFilter(GL::LINEAR_MIPMAP_LINEAR);
    GL::Texture2D::SetMagFilter(GL::LINEAR);
    if(mipmapCount == 1) {
        GL::Texture2D::GenerateMipmap();
    }

    return result;
}

GL::Texture2D ResourceManager::LoadTexture(int idx, uint32_t* mipmapCount)
{
    using namespace Resource;

    switch(idx) {
    case T_TEMPLATE:
        return Image::Load("cube", mipmapCount, nullptr);
    default:
    {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Texture resource ID: " << idx << " is valid, but not implemented";
        throw std::runtime_error(ss.str());
    }
    }
}

std::shared_ptr<GUI::Program>       ResourceManager::CreateProgram(int idx)
{
    using namespace Resource;

    switch(idx) {
    case P_GENERIC:
        return std::make_shared<GUI::Program>(GL::Program::Create(vertexShader, nullptr, fragmentShader), true);
    default:
    {
        std::stringstream ss;
        ss << "ResourceManager failed to create model. Shader resource ID: " << idx << " is valid, but not implemented";
        throw std::runtime_error(ss.str());
    }
    }
}


} // namespace Tetris
