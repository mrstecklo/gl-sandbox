#include "Tetris/Scene.h"
#include "Image.h"
#include "MainWindow.h"

namespace Tetris {


const GLchar Scene::vertexShader[] =
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

const GLchar Scene::fragmentShader[] = 
"#version 330 core\n"
"in vec2 UV;"
"out vec3 color;"
"uniform sampler2D texSampler;"
"void main() {"
"color = texture( texSampler, UV ).rgb;;"
"}";

const GLfloat Scene::g_vertex_buffer_data[numVertices][vertexSize] = { 
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

const GLfloat Scene::g_uv_buffer_data[numVertices][UVSize] = {
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

void Scene::InitImpl()
{
    GL::ClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    GL::EnableDepthTest();

    arr = GL::VertexArray();
    arr.Bind();

    auto p = GL::Program::Create(vertexShader, nullptr, fragmentShader);

    uint32_t mipmapCount = 1;
    bool invertV = false;
    auto texture = Image::Load("cube", &mipmapCount, &invertV);
    GL::Texture2D::SetMinFilter(GL::LINEAR_MIPMAP_LINEAR);
    GL::Texture2D::SetMagFilter(GL::LINEAR);
    if(mipmapCount == 1) {
        GL::Texture2D::GenerateMipmap();
    }

    if(invertV) {
        GL::UniformMatrix3x2f UVMat(p.GetUniformLocation("UVMat"));
        static const float ivertedUVMat[3][2] = {
            {1.f,  0.f},
            {0.f, -1.f},
            {0.f,  1.f}
        };
        p.Use();
        UVMat.Set(ivertedUVMat);
    }

    program.Init(std::move(p));

    cube.Init(
        GUI::ArrayBufferFloat(g_vertex_buffer_data, GL::STATIC_DRAW),
        GUI::ArrayBufferFloat(g_uv_buffer_data, GL::STATIC_DRAW),
        std::move(texture),
        &program
    );

    texture = Image::Load("cube", &mipmapCount, &invertV);
    GL::Texture2D::SetMinFilter(GL::LINEAR_MIPMAP_LINEAR);
    GL::Texture2D::SetMagFilter(GL::LINEAR);
    if(mipmapCount == 1) {
        GL::Texture2D::GenerateMipmap();
    }

    triangle.Init(
        GUI::ArrayBufferFloat(3, 3, &g_vertex_buffer_data[0][0], GL::STATIC_DRAW),
        GUI::ArrayBufferFloat(3, 2, &g_uv_buffer_data[0][0], GL::STATIC_DRAW),
        std::move(texture),
        &program
    );

    triangle.SetPosition(glm::vec3(0.f,3.f, 5.f));

    camera.FPLookAt(
        glm::vec3(5.f, 2.f, -10.f),
        glm::vec3(0.f, 0.f, 0.f));

    SetMouseMode(GUI::MouseMode::CENTERED);
}

void Scene::ResizeImpl(int width, int height)
{
    camera.SetWindowSize(static_cast<float>(width), static_cast<float>(height));
    glViewport(0, 0, width, height);
}

void Scene::ProcessInputImpl(const Util::PointD& cursor, std::chrono::microseconds timeSinceLastFrame)
{
    const auto microseconds = static_cast<float>(timeSinceLastFrame.count());

    const auto mouseUnit = microseconds * mouseSpeed;

    const auto precession = mouseUnit * static_cast<float>(cursor.x);
	const auto nutation   = mouseUnit * static_cast<float>(cursor.y);

    camera.FPRotate(precession, nutation);

    triangle.Rotate(glm::quat(1.f, 0.f, mouseUnit * 0.1f, mouseUnit * 0.1f));


    const auto keyboardUnit = microseconds * speed;

    auto forward = keyboardUnit * camera.GetDirection();
    auto right = keyboardUnit * camera.GetRight();

    if(GetWindow()->GetKey(GLFW_KEY_UP) == GLFW::PRESS) {
        camera.Translate(forward);
    }

    if(GetWindow()->GetKey(GLFW_KEY_DOWN) == GLFW::PRESS) {
        camera.Translate(-forward);
    }

    if(GetWindow()->GetKey(GLFW_KEY_RIGHT) == GLFW::PRESS) {
        camera.Translate(right);
    }

    if(GetWindow()->GetKey(GLFW_KEY_LEFT) == GLFW::PRESS) {
        camera.Translate(-right);
    }
}

void Scene::ForEachObjectImpl(ObjectCb cb) const
{
    cb(cube);
    cb(triangle);
}

void Scene::ForEachModelImpl(ModelCb cb) const
{
    cb(cube);
    cb(triangle);
}

} // namespace Tetris
