#include "Scene.h"
#include "GUI/MainWindow.h"

namespace Tetris {

void Scene::InitImpl()
{
    GL::ClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    GL::EnableDepthTest();

    arr = GL::VertexArray();
    arr.Bind();

    using namespace Resource;

    cube = resources.CreateModel(V_CUBE, T_TEMPLATE, P_GENERIC);
    triangle = resources.CreateModel(V_TRIANGLE, T_TEMPLATE, P_GENERIC);

    triangle.SetPosition(glm::vec3(0.f,0.f, 0.f));

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
