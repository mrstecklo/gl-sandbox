#include "GLObject.h"
#include <utility>

namespace GL {

ObjectBase::ObjectBase(ObjectBase&& other) :
    handle(other.Release())
{
}

ObjectBase& ObjectBase::operator=(ObjectBase&& other)
{
    if(this != &other) {
        std::swap(handle, other.handle);
    }
    return *this;
}

GLuint ObjectBase::Release()
{
    return std::exchange(handle, 0);
}

} // namespace GL