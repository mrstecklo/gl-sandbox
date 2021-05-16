#pragma once

#include "MainWindowFwd.h"
#include "Scene.h"
#include "GLFW/Window.h"

namespace GUI {

class MainWindow : public GLFW::Window {
public:
    using Clock = std::chrono::system_clock;

    MainWindow(GLFW::Window&& other, std::unique_ptr<Scene>&& s);
    ~MainWindow() override;

private:
    void OnRender() override;
    void OnResize(int width, int height) override;

    std::unique_ptr<Scene> scene;
    Clock::time_point   lastFrame = Clock::time_point::max();

    std::size_t count = 0;
};

} // namespace GUI
