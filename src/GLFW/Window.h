#pragma once

#include "Monitor.h"
#include "WindowCreatorFwd.h"
#include "Input.h"
#include "Util/Noncopyable.h"
#include "Util/Point.h"

namespace GLFW {

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

    KeyState GetKey(int key)                            const { return static_cast<KeyState>(glfwGetKey(handle, key)); }

    bool ShouldClose()                                  const { return glfwWindowShouldClose(handle) != 0; }

    void GetWindowSize(int* width, int* height)         const { glfwGetWindowSize(handle, width, height); }
    Util::Point GetWindowSize()                         const { Util::Point s; GetWindowSize(&s.x, &s.y); return s; }

    void GetFrameBufferSize(int* width, int* height)    const { glfwGetFramebufferSize(handle, width, height);}
    Util::Point GetFrameBufferSize()                    const { Util::Point s; GetFrameBufferSize(&s.x, &s.y); return s; }

    void GetCursorPos(double* x, double* y)             const { glfwGetCursorPos(handle, x, y); }
    Util::PointD GetCursorPos()                         const { Util::PointD p; GetCursorPos(&p.x, &p.y); return p; }

    void SetCursorPos(double x, double y)                     { glfwSetCursorPos(handle, x, y); }

    bool IsFocused() const { return glfwGetWindowAttrib(handle, GLFW_FOCUSED) != 0;}

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
    Util::Point size{0, 0};
};

} // namespace GLFW
