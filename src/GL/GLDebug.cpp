#include "GLDebug.h"

namespace GL {
namespace Debug {

void Enable()
{
    glEnable(GL_DEBUG_OUTPUT);
}

void Disable()
{
    glDisable(GL_DEBUG_OUTPUT);
}

void RegisterCallback(Callback cb, void* userParam)
{
    glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(cb), userParam);
}

} // namespace Debug

const char* ToString(Debug::Severity s)
{
    using namespace Debug;
    switch(s) {
    case Severity::HIGH:
        return "High";
    case Severity::MEDIUM:
        return "Medium";
    case Severity::LOW:
        return "Low";
    case Severity::NOTIFICATION:
        return "Notification";
    default:
        return "Invalid";
    }
}

const char* ToString(Debug::Type t)
{
    using namespace Debug;
    switch(t) {
    case Type::ERROR:
        return "Error";
    case Type::DEPRECATED_BEHAVIOR:
        return "Deprecated behavior";
    case Type::UNDEFINED_BEHAVIOR:
        return "Undefined behavior";
    case Type::PORTABILITY:
        return "Portability";
    case Type::PERFORMANCE:
        return "Performance";
    case Type::MARKER:
        return "Marker";
    case Type::PUSH_GROUP:
        return "Push group";
    case Type::POP_GROUP:
        return "Pop group";
    case Type::OTHER:
        return "Other";
    default:
        return "Invalid";
    }
}

const char* ToString(Debug::Source s)
{
    using namespace Debug;
    switch(s) {
    case Source::API:
        return "API";
    case Source::WINDOW_SYSTEM:
        return "Window system";
    case Source::SHADER_COMPILER:
        return "Shader compiler";
    case Source::THIRD_PARTY:
        return "Third party";
    case Source::APPLICATION:
        return "Application";
    case Source::OTHER:
        return "Other";
    default:
        return "Invalid";
    }
}

} // namespace GL