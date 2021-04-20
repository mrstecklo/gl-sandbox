#include "GLDebug.h"

using namespace GL::Debug;

const char* ToString(Severity s)
{
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

const char* ToString(Type t)
{
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

const char* ToString(Source s)
{
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
