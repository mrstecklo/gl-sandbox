#pragma once

#include "GLFWMonitor.h"
#include "GLFWWindowCreatorFwd.h"
#include "Noncopyable.h"
#include <atomic>
#include <mutex>

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

    virtual ~Window();
    Window(Window&& other);
    Window& operator=(Window&& other);

    GLFWwindow* Get() { return handle; }
    bool IsOpen() const { return handle != nullptr; }
    
    void MakeCurrent() { glfwMakeContextCurrent(handle); }

    void SetInputMode(CursorInputMode mode, CursorInputModeValue value) { glfwSetInputMode(handle, mode, value); }
    void SetInputMode(BooleanInputMode mode, bool value)                { glfwSetInputMode(handle, mode, value); }

    void SwapBuffers() { glfwSwapBuffers(handle); }

    KeyState GetKey(int key) { return static_cast<KeyState>(glfwGetKey(handle, key)); }

    bool ShouldClose() const { return glfwWindowShouldClose(handle) != 0; }

    void GetWindowSize(int* width, int* height) const { glfwGetWindowSize(handle, width, height); }
    void GetFrameBufferSize(int* width, int* height) const { glfwGetFramebufferSize(handle, width, height);}

    void Render();

private:

    Window(
        int  	        width,
		int             height,
		const char*  	title,
		Monitor*  	    monitor,
		Window*         share);

    virtual void OnRender() {}
    virtual void OnResize(int /* width */, int /* height */) {};

    static void swap(Window& l, Window& r);

    static void FramebufferSizeCallback(GLFWwindow* handle, int width, int height);

    struct Size {
        int width;
        int height;
    };

    mutable std::mutex  handleMx;
    GLFWwindow*         handle = nullptr;
    std::atomic<Size>   size{{0, 0}};
};

} // namespace GLFW
