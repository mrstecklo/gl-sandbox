#include "Scene.h"
#include "GUI/MainWindow.h"
#include <iostream>

namespace Tetris {

constexpr std::chrono::microseconds Scene::tickDuration;

Scene::Scene() :
    map(width, height)
{
}

void Scene::InitImpl()
{
    GL::ClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    GL::EnableDepthTest();

    arr = GL::VertexArray();
    arr.Bind();

    using namespace Resource;

    FPLookingCube = lookingCube = cube = scube = resources.CreateModel(V_CUBE, T_AXIS, P_GENERIC);
    triangle = resources.CreateModel(V_TRIANGLE, T_TEMPLATE, P_GENERIC);

    triangle.SetPosition(glm::vec3(0.f,0.f, 2.f));

    camera.FPLookAt(
        glm::vec3(5.f, 2.f, 10.f),
        glm::vec3(0.f, 0.f, 0.f));

    //cube.SetPosition(glm::vec3(3.f, 0.f, 0.f));

    lookingCube.SetPosition(glm::vec3(0.f, 0.f, 5.f));
    FPLookingCube.SetPosition(glm::vec3(3.f, 0.f, 5.f));

    SetMouseMode(GLFW::CURSOR_DISABLED);
}

void Scene::ResizeImpl(int width, int height)
{
    camera.SetWindowSize(static_cast<float>(width), static_cast<float>(height));
    glViewport(0, 0, width, height);
}

void Scene::ProcessInputImpl(const Util::PointD& cursor, std::chrono::microseconds timeSinceLastFrame)
{
    if(GetWindow()->GetKey(GLFW_KEY_ESCAPE) == GLFW::PRESS) {
        GetWindow()->SetShouldClose(true);
        return;
    }

    counter += timeSinceLastFrame;
    if(counter >= tickDuration) {
        counter %= tickDuration;
        map.Tick(Map::Input::NIL);
    }

    const auto microseconds = static_cast<float>(timeSinceLastFrame.count());

    const auto mouseUnit = microseconds * mouseSpeed;

    const auto precession = -mouseUnit * static_cast<float>(cursor.x);
	const auto nutation   = -mouseUnit * static_cast<float>(cursor.y);

    //std::cout << "cursor: " << cursor.x << 'x' << cursor.y 
    //    << ", " << (cursor.x - prevCursor.x) << 'x' << (cursor.y - prevCursor.y) << std::endl;

    //prevCursor = cursor;
    

    camera.FPRotate(precession, nutation);
    //camera.Rotate(glm::quat(1.f, 0.f, precession, 0.f));
    //camera.MSRotate(glm::quat(1.f, nutation, 0.f, 0.f));

    //triangle.Rotate(glm::quat(1.f, 0.f, mouseUnit * 0.1f, 0.f));

    triangle.FPRotate(mouseUnit, 0.f);

    //cube.FPLookAt(camera.GetPosition());

    //cube.Rotate(glm::quat(1.f, 0.f, mouseUnit, 0.f));
    //cube.MSRotate(glm::quat(1.f, 0.f, 0.f, mouseUnit * 5.f));

    const auto keyboardUnit = microseconds * speed;

    auto forward = keyboardUnit * camera.GetDirection();
    auto right = keyboardUnit * camera.GetRight();
    auto up = keyboardUnit * camera.GetUp();

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

    if(GetWindow()->GetKey(GLFW_KEY_KP_ADD) == GLFW::PRESS) {
        camera.Translate(up);
    }

    if(GetWindow()->GetKey(GLFW_KEY_KP_SUBTRACT) == GLFW::PRESS) {
        camera.Translate(-up);
    }

    lookingCube.LookAt(camera.GetPosition(), glm::vec3(0.f, 1.f, 0.f));
    FPLookingCube.FPLookAt(camera.GetPosition());
    
}

void Scene::ForEachModelImpl(ModelCb cb)
{
    auto& grid = map.GetGrid();

    static glm::vec3 mapBase(0.f, 0.f, -10.f);
    static float step = 2.5f;
    
    for(std::size_t j = 0; j < grid.height(); ++j) {
        auto row = grid[j];
        for(std::size_t i = 0; i < grid.width(); ++i) {
            if(row[i] != 0) {
                const auto pos = mapBase + glm::vec3(step * i, step * j, 0.f);
                cube.SetPosition(pos);
                cb(cube);
            }
        }
    }

    auto& figure = map.GetFigure();

    for(std::size_t i = 0; i < Figure::tetra; ++i) {
        const auto p = figure[i];
        const auto pos = mapBase + glm::vec3(step * p.x, step * p.y, 0.f);
        cube.SetPosition(pos);
        cb(cube);
    }

    cb(scube);
    cb(triangle);
    cb(lookingCube);
    cb(FPLookingCube);
}

} // namespace Tetris
