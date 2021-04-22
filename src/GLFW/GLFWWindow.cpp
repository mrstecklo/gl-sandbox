#include "GLFWWindow.h"

namespace GLFW {

Window::Window(
        int  	        width,
        int             height,
        const char*  	title,
        Monitor*  	    monitor,
        Window*         share) :
    handle(glfwCreateWindow( width, height, title, monitor ? monitor->Get() : nullptr, share ? share->handle : nullptr))
{
    if(handle) {
        glfwSetWindowUserPointer(handle, this); 

        Size s;
        GetFrameBufferSize(&s.width, &s.height);
        size = s;

        glfwSetFramebufferSizeCallback(handle, FramebufferSizeCallback);
    }
}

Window::~Window() {
    glfwDestroyWindow(handle);
}

void Window::swap(Window& l, Window& r)
{
    std::unique_lock<std::mutex> lock1(l.handleMx, std::defer_lock);
    std::unique_lock<std::mutex> lock2(r.handleMx, std::defer_lock);
    std::lock(lock1, lock2);

    std::swap(l.handle, r.handle);
    if(l.handle) {
        glfwSetWindowUserPointer(l.handle, &l); 
    }
    if(r.handle) {
        glfwSetWindowUserPointer(r.handle, &r); 
    }
    l.size = r.size.exchange(l.size);
}

Window::Window(Window&& other)
{
    swap(*this, other);
}

Window& Window::operator=(Window&& other)
{
    swap(*this, other);
    return *this;
}

void Window::Render()
{
    MakeCurrent();
    auto s = size.exchange({0, 0});
    if(s.width != 0 && s.height != 0) {
        OnResize(s.width, s.height);
    }
    OnRender();
    SwapBuffers();    
}

void Window::FramebufferSizeCallback(GLFWwindow* handle, int width, int height)
{
    while(true) {
        auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
        std::unique_lock<std::mutex> guard(w->handleMx);
        if(w->handle == handle) {
            w->size = Size{width, height};
            return;
        }
    }
}

} // namespace GLFW
