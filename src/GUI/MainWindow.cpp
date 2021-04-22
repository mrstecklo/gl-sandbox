#include "MainWindow.h"
#include "Image.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace GUI {

const GLchar MainWindow::vertexShader[] =
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

const GLchar MainWindow::fragmentShader[] = 
"#version 330 core\n"
"in vec2 UV;"
"out vec3 color;"
"uniform sampler2D texSampler;"
"void main() {"
"color = texture( texSampler, UV ).rgb;;"
"}";

const GLfloat MainWindow::g_vertex_buffer_data[numVertices][vertexSize] = { 
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

const GLfloat MainWindow::g_uv_buffer_data[numVertices][UVSize] = {
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

MainWindow::MainWindow(GLFW::Window&& other) :
    GLFW::Window(std::move(other))
{
    MakeCurrent();

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
    }

    GL::ClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    GL::EnableDepthTest();

    arr = GL::VertexArray();
    arr.Bind();

    vertexBuffer = GL::ArrayBuffer();
    vertexBuffer.Bind();
    GL::ArrayBuffer::Data(g_vertex_buffer_data, GL::STATIC_DRAW);

    UVBuffer = GL::ArrayBuffer();
    UVBuffer.Bind();
    GL::ArrayBuffer::Data(g_uv_buffer_data, GL::STATIC_DRAW);

    uint32_t mipmapCount = 1;
    bool invertV = false;
    texture = Image::Load("cube", &mipmapCount, &invertV);
    GL::Texture2D::SetMinFilter(GL::LINEAR_MIPMAP_LINEAR);
    GL::Texture2D::SetMagFilter(GL::LINEAR);
    if(mipmapCount == 1) {
        GL::Texture2D::GenerateMipmap();
    }

    program = GL::Program::Create(vertexShader, nullptr, fragmentShader);
    MVP = GL::UniformMatrix4f(program.GetUniformLocation("MVP"));

    if(invertV) {
        GL::UniformMatrix3x2f UVMat(program.GetUniformLocation("UVMat"));
        static const float ivertedUVMat[3][2] = {
            {1.f,  0.f},
            {0.f, -1.f},
            {0.f,  1.f}
        };
        program.Use();
        UVMat.Set(ivertedUVMat);
    }

   

    CubeMat = glm::mat4(1.0f);
    TriangleMat = glm::translate(glm::vec3(2.f, 0.f, -1.f)) * glm::scale(glm::vec3(2.f, 2.f, 2.f));


    view = glm::lookAt(
			glm::vec3(4,3,-3),
			glm::vec3(0,0,0),
			glm::vec3(0,1,0)
			);
}

MainWindow::~MainWindow()
{
    MakeCurrent();
}

void MainWindow::OnResize(int width, int height)
{
    const auto ratio = static_cast<float>(width) / static_cast<float>(height);
    projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.f);
    VP = projection * view;
    glViewport(0, 0, width, height);
}

void MainWindow::OnRender()
{
    glm::mat4 CubeMVP = VP * CubeMat;
    glm::mat4 TriangleMVP = VP * TriangleMat;

    GL::Clear( GL::COLOR_BUFFER_BIT, GL::DEPTH_BUFFER_BIT );

    program.Use();
    MVP.Set(&CubeMVP[0][0]);

    GL::VertexAttribArray vertices(attrVertices);
    GL::VertexAttribArray UVs(attrUV);

    vertexBuffer.Bind();
    GL::VertexAttribPointer(
        attrVertices,
        vertexSize,
        GL::FLOAT,
        0,
        nullptr
    );

    UVBuffer.Bind();
    GL::VertexAttribPointer(
        attrUV,
        UVSize,
        GL::FLOAT,
        0,
        nullptr
    );

    GL::DrawArrays(GL::TRIANGLES, numVertices);

    MVP.Set(&TriangleMVP[0][0]);

    GL::DrawArrays(GL::TRIANGLES, 3);
}

} // namespace GUI
