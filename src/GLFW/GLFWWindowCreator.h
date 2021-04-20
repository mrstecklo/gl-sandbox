#pragma once

#include "GLFWWindow.h"
#include  <limits.h>

namespace GLFW {

enum IntegralHint : int {
    RED_BITS = GLFW_RED_BITS,
    GREEN_BITS= GLFW_GREEN_BITS,
    BLUE_BITS = GLFW_BLUE_BITS,
    ALPHA_BITS = GLFW_ALPHA_BITS,
    DEPTH_BITS = GLFW_DEPTH_BITS,
    STENCIL_BITS = GLFW_STENCIL_BITS,
    ACCUM_RED_BITS = GLFW_ACCUM_RED_BITS,
    ACCUM_GREEN_BITS = GLFW_ACCUM_GREEN_BITS,
    ACCUM_BLUE_BITS = GLFW_ACCUM_BLUE_BITS,
    ACCUM_ALPHA_BITS = GLFW_ACCUM_ALPHA_BITS,
    AUX_BUFFERS = GLFW_AUX_BUFFERS,
    SAMPLES = GLFW_SAMPLES,
    REFRESH_RATE = GLFW_REFRESH_RATE
};

enum BooleanHint : int {
    RESIZABLE = GLFW_RESIZABLE,
    VISIBLE = GLFW_VISIBLE,
    DECORATED = GLFW_DECORATED,
    FOCUSED = GLFW_FOCUSED,
    AUTO_ICONIFY = GLFW_AUTO_ICONIFY,
    FLOATING = GLFW_FLOATING,
#ifdef GLFW_MAXIMIZED
    MAXIMIZED = GLFW_MAXIMIZED,
#endif // GLFW_MAXIMIZED
    STEREO = GLFW_STEREO,
    SRGB_CAPABLE = GLFW_SRGB_CAPABLE,
    DOUBLEBUFFER = GLFW_DOUBLEBUFFER,
    OPENGL_FORWARD_COMPAT = GLFW_OPENGL_FORWARD_COMPAT,
    OPENGL_DEBUG_CONTEXT = GLFW_OPENGL_DEBUG_CONTEXT,
#ifdef GLFW_COCOA_RETINA_FRAMEBUFFER
    COCOA_RETINA_FRAMEBUFFER = GLFW_COCOA_RETINA_FRAMEBUFFER,
#endif // GLFW_COCOA_RETINA_FRAMEBUFFER
#ifdef GLFW_COCOA_GRAPHICS_SWITCHING
    COCOA_GRAPHICS_SWITCHING = GLFW_COCOA_GRAPHICS_SWITCHING,
#endif
};

enum ClientAPIHint : int { CLIENT_API = GLFW_CLIENT_API };

enum ContextCreationAPIHint : int { 
#ifdef GLFW_CONTEXT_CREATION_API
    CONTEXT_CREATION_API = GLFW_CONTEXT_CREATION_API
#endif
};

enum ContextVersionMajorHint : int { CONTEXT_VERSION_MAJOR = GLFW_CONTEXT_VERSION_MAJOR }; 
enum ContextVersionMinorHint : int { CONTEXT_VERSION_MINOR = GLFW_CONTEXT_VERSION_MINOR };
enum ContextRobustnessHint : int { CONTEXT_ROBUSTNESS = GLFW_CONTEXT_ROBUSTNESS };
enum ContextReleaseBehaviorHint : int { CONTEXT_RELEASE_BEHAVIOR = GLFW_CONTEXT_RELEASE_BEHAVIOR };
enum OpenGLProfileHint : int { OPENGL_PROFILE = GLFW_OPENGL_PROFILE };

enum AnyHintValue : int { DONT_CARE = GLFW_DONT_CARE };

enum ContextRobustnessHintValue : int {
    NO_ROBUSTNESS = GLFW_NO_ROBUSTNESS,
    NO_RESET_NOTIFICATION = GLFW_NO_RESET_NOTIFICATION,
    LOSE_CONTEXT_ON_RESET = GLFW_LOSE_CONTEXT_ON_RESET
};

enum ContextReleaseBehaviorHintValue : int {
    ANY_RELEASE_BEHAVIOR = GLFW_ANY_RELEASE_BEHAVIOR,
    RELEASE_BEHAVIOR_FLUSH = GLFW_RELEASE_BEHAVIOR_FLUSH,
    RELEASE_BEHAVIOR_NONE = GLFW_RELEASE_BEHAVIOR_NONE
};

enum OpenGLProfileHintValue : int {
    OPENGL_ANY_PROFILE = GLFW_OPENGL_ANY_PROFILE,
    OPENGL_COMPAT_PROFILE = GLFW_OPENGL_COMPAT_PROFILE,
    OPENGL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE
};

class HintValueBase {
public:
    constexpr int Get() const { return value; }

protected:
    constexpr HintValueBase(int v) : value(v) {}

private:
    int value;
};

class IntegralHintValue : public HintValueBase {
public:
    constexpr IntegralHintValue(int v) :
        HintValueBase((v < 0 || v > INT_MAX) ? INT_MAX : v)
    {}

    constexpr IntegralHintValue(AnyHintValue v) : HintValueBase(v) {}
};

class BooleanHintValue : public HintValueBase {
public:
    constexpr BooleanHintValue(bool v) :
        HintValueBase(v) {}
};

enum DefaultWindowHintsType { DEFAULT_WINDOW_HINTS };

class WindowCreator {
public:
    friend class Scope;

    WindowCreator& Hint(IntegralHint hint, IntegralHintValue value)                               { glfwWindowHint (hint, value.Get()); return *this; }
    WindowCreator& Hint(BooleanHint hint, bool value)                                             { glfwWindowHint (hint, value); return *this; }
    WindowCreator& Hint(ContextVersionMajorHint hint, int value)                                  { glfwWindowHint (hint, value); return *this; }
    WindowCreator& Hint(ContextVersionMinorHint hint, int value)                                  { glfwWindowHint (hint, value); return *this; }
    WindowCreator& Hint(ContextRobustnessHint hint, ContextRobustnessHintValue value)             { glfwWindowHint (hint, value); return *this; }
    WindowCreator& Hint(ContextReleaseBehaviorHint hint, ContextReleaseBehaviorHintValue value)   { glfwWindowHint (hint, value); return *this; }
    WindowCreator& Hint(OpenGLProfileHint hint, OpenGLProfileHintValue value)                     { glfwWindowHint (hint, value); return *this; }

    Window Finalize(
        int  	        width,
		int             height,
		const char*  	title,
		Monitor*  	    monitor,
		Window*         share)
    {
        return Window(width, height, title, monitor, share);
    }

private:
    WindowCreator() = default;
    WindowCreator(DefaultWindowHintsType) { glfwDefaultWindowHints(); }
};

} // namespace GLFW
