#pragma once

#include "ScopeFwd.h"
#include <GLFW/glfw3.h>

namespace GLFW {

class Monitor {
public:
    friend class Scope;

    GLFWmonitor* Get() { return handle; }

private:
    static Monitor GetPrimary() { return Monitor(glfwGetPrimaryMonitor()); }
    static Monitor* GetAll(int* count)
    {
        static_assert(sizeof(Monitor) == sizeof(GLFWmonitor*) && alignof(Monitor) == alignof(GLFWmonitor*), "Monitor is more than just its underlying handle");
        return reinterpret_cast<Monitor*>(glfwGetMonitors(count)); 	
    }

    constexpr explicit Monitor(GLFWmonitor* h) : handle(h) {}

    GLFWmonitor* handle = nullptr;
};

} // namespace GLFW
