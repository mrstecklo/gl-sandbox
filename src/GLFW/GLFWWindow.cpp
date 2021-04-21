#include "GLFWWindow.h"

namespace GLFW {

std::map<GLFWwindow*, Window*> Window::instances;
std::mutex Window::mapMx;

Window::Window(
        int  	        width,
        int             height,
        const char*  	title,
        Monitor*  	    monitor,
        Window*         share) :
    handle(glfwCreateWindow( width, height, title, monitor ? monitor->Get() : nullptr, share ? share->handle : nullptr))
{
    if(handle) {
        std::unique_lock<std::mutex> guard(mapMx);
        instances[handle] = this;
        guard.unlock();
        glfwSetFramebufferSizeCallback(handle, FramebufferSizeCallback);
    }
}

Window::~Window() {
    if(handle) {
        glfwDestroyWindow(handle);
        std::lock_guard<std::mutex> guard(mapMx);
        instances.erase(handle);
    }
}

Window::Window(Window&& other)
{
    if(other.handle) {
        std::lock_guard<std::mutex> guard(mapMx);
        handle = std::exchange(other.handle, nullptr);
        instances[handle] = this;
    }
}

Window& Window::operator=(Window&& other)
{
    std::lock_guard<std::mutex> guard(mapMx);
    std::swap(handle, other.handle);
    instances[handle] = this;
    instances[other.handle] = &other;
    instances.erase(nullptr);
    return *this;
}

void Window::FramebufferSizeCallback(GLFWwindow* handle, int width, int height)
{
    std::unique_lock<std::mutex> guard(mapMx);
    auto it = instances.find(handle);
    if(it != instances.end()) {
        auto w = it->second;
        guard.unlock();
        w->Resize(width, height);
    }
}

} // namespace GLFW