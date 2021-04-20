#pragma once

#include "GLFWWindowCreator.h"
#include <atomic>

namespace GLFW {

class Scope : private Util::Noncopyable {
public:
    Scope();
    ~Scope();

    WindowCreator CreateWindow() { return WindowCreator(); }
    WindowCreator CreateWindow(DefaultWindowHintsType v) { return WindowCreator(v); }

    Monitor GetPrimaryMonitor() { return Monitor::GetPrimary(); }
    Monitor* GetMonitors(int* count) { return Monitor::GetAll(count); }

    void PollEvents() { glfwPollEvents(); }

private:
    static std::atomic<bool> instantiated;
};

} // namespace GLFW