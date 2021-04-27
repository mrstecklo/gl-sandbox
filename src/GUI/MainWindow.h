#pragma once

#include "GLCPP.h"
#include "GLFWWindow.h"
#include "Camera.h"
#include <chrono>

namespace GUI {

class MainWindow : public GLFW::Window {
public:
    MainWindow(GLFW::Window&& other);
    ~MainWindow() override;

private:
    void OnRender() override;
    void OnResize(int width, int height) override;

    static constexpr GLint vertexSize = 3;
    static constexpr GLint UVSize = 2;
    static constexpr GLsizei numVertices = 36;

    static constexpr auto attrVertices = GL::VertexAttrib(0);
    static constexpr auto attrUV = GL::VertexAttrib(1);

    static const GLchar vertexShader[];
    static const GLchar fragmentShader[];

    static const GLfloat g_vertex_buffer_data[numVertices][vertexSize];
    static const GLfloat g_uv_buffer_data[numVertices][UVSize];

    GL::VertexArray arr {nullptr};
    GL::ArrayBuffer vertexBuffer {nullptr};
    GL::ArrayBuffer UVBuffer {nullptr};
    GL::Texture2D texture {nullptr};

    GL::Program program {nullptr};
    GL::UniformMatrix4f MVP{0};

    glm::mat4 VP;
    glm::mat4 CubeMat;
    glm::mat4 TriangleMat;

    Camera    camera;

    using Clock = std::chrono::system_clock;
    Clock::time_point   lastFrame = Clock::time_point::min();
    float               mouseSpeed = 0.005f;
};

} // namespace GUI
