#pragma once

#include "GLFWMonitor.h"
#include "GLFWWindowCreatorFwd.h"
#include "Noncopyable.h"

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

template<class T>
struct PointT {
    T x;
    T y;

    bool operator==(const PointT<T>& r) const { return x == r.x && y == r.y; }
    bool operator!=(const PointT<T>& r) const { return !(*this == r); }
};

using Point = PointT<int>;
using PointD = PointT<double>;

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

    void SwapBuffers()                                        { glfwSwapBuffers(handle); }

    KeyState GetKey(int key)                                  { return static_cast<KeyState>(glfwGetKey(handle, key)); }

    bool ShouldClose()                                  const { return glfwWindowShouldClose(handle) != 0; }

    void GetWindowSize(int* width, int* height)         const { glfwGetWindowSize(handle, width, height); }
    Point GetWindowSize()                               const { Point s; GetWindowSize(&s.x, &s.y); return s; }

    void GetFrameBufferSize(int* width, int* height)    const { glfwGetFramebufferSize(handle, width, height);}
    Point GetFrameBufferSize()                          const { Point s; GetFrameBufferSize(&s.x, &s.y); return s; }

    void GetCursorPos(double* x, double* y)             const { glfwGetCursorPos(handle, x, y); }
    PointD GetCursorPos()                               const { PointD p; GetCursorPos(&p.x, &p.y); return p; }

    void SetCursorPos(double x, double y)                     { glfwSetCursorPos(handle, x, y); }

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
    void SetThisAsUserPointer() {if(handle) glfwSetWindowUserPointer(handle, this); }

    GLFWwindow* handle = nullptr;
    Point       size{0, 0};
};

} // namespace GLFW
