#include "Window.h"
#include <utility>

namespace GLFW {

Window::Window(
        int  	        width,
        int             height,
        const char*  	title,
        Monitor*  	    monitor,
        Window*         share) :
    handle(glfwCreateWindow( width, height, title, monitor ? monitor->Get() : nullptr, share ? share->handle : nullptr))
{
    SetThisAsUserPointer();
}

Window::~Window()
{
    glfwDestroyWindow(handle);
}

void Window::swap(Window& l, Window& r)
{
    std::swap(l.handle, r.handle);
    l.SetThisAsUserPointer();
    r.SetThisAsUserPointer();
    std::swap(l.size, r.size);
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
    auto s = GetFrameBufferSize();
    if(s != size) {
        size = s;
        OnResize(size.x, size.y);
    }
    OnRender();
    SwapBuffers();    
}

} // namespace GLFW
