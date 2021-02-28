#pragma once

#include <GL/glew.h>
#include "Noncopyable.h"

namespace GL {

class ObjectBase : private Util::Noncopyable {
public:
    bool IsValid() const { return handle != 0; }

protected:
    ObjectBase(GLuint value) : handle(value) {}
    ObjectBase(std::nullptr_t) : handle(0) {}

    ObjectBase(ObjectBase&& other);
    ObjectBase& operator=(ObjectBase&& other);

    const GLuint& Get() const { return handle; }
          GLuint& Get()       { return handle; }
          
    GLuint Release();

private:
    GLuint handle;
};

template<class T>
class Object : public ObjectBase {
public:

    ~Object();
    void Reset();

protected:
    Object(std::nullptr_t p) : ObjectBase(p) {}
    Object(GLuint value) : ObjectBase(value) {}

    Object(Object&& other) = default;
    Object& operator=(Object&& other) = default;

private:

};

} // namespace GL

#include "GLObject.inl"