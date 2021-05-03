#pragma once

#include <GLFW/glfw3.h>

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

} // namespace GLFW