#pragma once

#include "GLFWMonitor.h"
#include "GLFWWindowCreatorFwd.h"
#include "Noncopyable.h"
#include <utility>

namespace GLFW {

enum CursorInputMode : int { CURSOR = GLFW_CURSOR };
enum BooleanInputMode : int {
    STICKY_KEYS = GLFW_STICKY_KEYS,
    STICKY_MOUSE_BUTTONS = GLFW_STICKY_MOUSE_BUTTONS,
#ifdef GLFW_LOCK_KEY_MODS
    LOCK_KEY_MODS = GLFW_LOCK_KEY_MODS,
#endif // GLFW_LOCK_KEY_MODS
#ifdef GLFW_RAW_MOUSE_MOTION
    RAW_MOUSE_MOTION = GLFW_RAW_MOUSE_MOTION
#endif // GLFW_RAW_MOUSE_MOTION
};

enum CursorInputModeValue : int {
    CURSOR_NORMAL = GLFW_CURSOR_NORMAL,
    CURSOR_HIDDEN = GLFW_CURSOR_HIDDEN,
    CURSOR_DISABLED = GLFW_CURSOR_DISABLED
};

enum KeyState : int {
    PRESS = GLFW_PRESS,
    RELEASE = GLFW_RELEASE
};

class Window : private Util::Noncopyable {
public:
    friend class WindowCreator;

    ~Window() { glfwDestroyWindow(handle); }

    Window(Window&& other) :
        handle(std::exchange(other.handle, nullptr))
    {}

    Window& operator=(Window&& other)
    {
        std::swap(handle, other.handle);
        return *this;
    }

    GLFWwindow* Get() { return handle; }
    bool IsOpen() const { return handle != nullptr; }
    
    void MakeCurrent() { glfwMakeContextCurrent(handle); }

    void SetInputMode(CursorInputMode mode, CursorInputModeValue value) { glfwSetInputMode(handle, mode, value); }
    void SetInputMode(BooleanInputMode mode, bool value)                { glfwSetInputMode(handle, mode, value); }

    void SwapBuffers() { glfwSwapBuffers(handle); }

    KeyState GetKey(int key) { return static_cast<KeyState>(glfwGetKey(handle, key)); }

    bool ShouldClose() const { return glfwWindowShouldClose(handle) != 0; }

    void GetSize(int* width, int* height) const { glfwGetWindowSize(handle, width, height); }

    void Render()
    {
        RenderImpl();
        SwapBuffers();    
    }

private:
    Window(
        int  	        width,
		int             height,
		const char*  	title,
		Monitor*  	    monitor,
		Window*         share) :
        handle(glfwCreateWindow( width, height, title, monitor ? monitor->Get() : nullptr, share ? share->handle : nullptr))
    {}

    virtual void RenderImpl() {}

    GLFWwindow* handle = nullptr;
};

} // namespace GLFW
