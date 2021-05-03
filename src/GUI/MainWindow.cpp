
#include "MainWindow.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace GUI {

MainWindow::MainWindow(GLFW::Window&& other, std::unique_ptr<Scene>&& s) :
    GLFW::Window(std::move(other)),
    scene(std::move(s))
{
    MakeCurrent();

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
    }

    scene->Init(this);
}

MainWindow::~MainWindow()
{
    MakeCurrent();
    scene.reset();
}

void MainWindow::OnResize(int width, int height)
{
    scene->Resize(width, height);
}

void MainWindow::OnRender()
{
    auto now = Clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>((now > lastFrame) ? (now - lastFrame) : Clock::duration(0));

    lastFrame = now;

    auto mouse = GetCursorPos();
    const auto window = GetWindowSize();

    switch(scene->GetMouseMode()) {
    case MouseMode::DISABLED:
        mouse = {0, 0};
        break;
    case MouseMode::CENTERED:
    {
        const Util::PointD center {static_cast<double>(window.x / 2), static_cast<double>(window.y / 2)};
        SetCursorPos(center.x, center.y);
        mouse -= center;
        break;
    }
    default:
    case MouseMode::FREE:
        break;
    };

    scene->ProcessInput(mouse, duration);

    GL::Clear( GL::COLOR_BUFFER_BIT, GL::DEPTH_BUFFER_BIT );

    GL::VertexAttribArray vertices(Program::attrVertices);
    GL::VertexAttribArray UVs(Program::attrUV);

    auto VP = scene->GetCamera().GetProjectionMat() * scene->GetCamera().GetViewMat();

    scene->ForEachModel(
        [&VP](const Model& m)
        {
            auto MVP = VP * glm::translate(m.GetPosition()) * glm::mat4_cast(glm::inverse(m.GetRotation()));
            m.GetProgram()->Use();
            m.GetProgram()->GetMVP().Set(&MVP[0][0]);

            m.GetTexture()->Bind();

            m.GetUVBuffer()->Bind();
            m.GetUVBuffer()->VertexAttribPointer(Program::attrUV);

            m.GetVertexBuffer()->Bind();
            m.GetVertexBuffer()->VertexAttribPointer(Program::attrVertices);

            m.GetVertexBuffer()->DrawArrays(GL::TRIANGLES);
        }
    );
}

} // namespace GUI
