#pragma once

#include <GL/glew.h>
#include <vector>
#include "Noncopyable.h"
#include "ZeroOutAllocator.h"

namespace GL {

class ObjectBase : private Util::Noncopyable {
public:
    bool IsValid() const { return handle != 0; }

protected:
    explicit ObjectBase(GLuint value) : handle(value) {}
    explicit ObjectBase(std::nullptr_t) : handle(0) {}

    ObjectBase(ObjectBase&& other);
    ObjectBase& operator=(ObjectBase&& other);

    const GLuint& Get() const { return handle; }
          GLuint& Access()    { return handle; }
          
    GLuint Release();

private:
    GLuint handle;
};

// GL object prototype
// T must derive from Object<T>
// T must provide static void Destroy(GLuint) function (e.g. glDeleteProgram() or a wrapper of glDeleteTextures())
template<class T>
class Object : public ObjectBase {
public:
    ~Object();
    void Reset();

protected:
    explicit Object(GLuint value) : ObjectBase(value) {}
    explicit Object(std::nullptr_t p) : ObjectBase(p) {}

    Object(Object&& other) = default;
    Object& operator=(Object&& other) = default;

private:

};

// Wrapper layer for most GL objects (except for Program and Shader)
// Allows creating several objects at once with functions like glGenBuffers
// Destructors are called for each object separately though
// T must derive from ObjectArray<T>
// T must provide static void DestroyArray(GLsizei, const GLuint*) function (e.g. glDeleteTextures())
// T must provide static void CreateArray(GLsizei, GLuint*) function (e.g. glGenTextures())
// T must provide either 
//      static void BindImpl(GLuint handle) (e.g glBindVertexArray or wrapper of glBindTexture)
// or   static void BindImpl(GLenum target, GLuint handle) (e.g glBindTexture)
template<class T>
class ObjectArray : public Object<ObjectArray<T> > {
public:
    typedef Object<ObjectArray<T> > Obj;
    // Using ZeroOutAllocator to prevent calling of default constructor
    typedef Util::ZeroOutAllocator<T> Allocator;
    typedef std::vector<T, Allocator> Container;

    friend void Obj::Reset();

    static Container Create(GLsizei n);

protected:
    explicit ObjectArray(GLuint value) : Obj(value) {}
    explicit ObjectArray(std::nullptr_t p) : Obj(p) {}
    ObjectArray();

    ObjectArray(ObjectArray&& other) = default;
    ObjectArray& operator=(ObjectArray&& other) = default;

    void Bind();
    void Bind(GLenum target);

    static void BindDefault();
    static void BindDefault(GLenum target);

private:
    static void Destroy(const GLuint& value);
};


} // namespace GL

#include "GLObject.inl"