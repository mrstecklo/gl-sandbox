#pragma once

namespace GL {

template<class T>
Object<T>::~Object()
{
    Reset();
}

template<class T>
void Object<T>::Reset()
{
    static_assert(std::is_base_of<Object<T>, T>::value, "Usage: class Shader : public Object<Shader>");
    if(IsValid()) {
        T::Destroy(Get());
        Release();
    }
}

template<class T>
typename ObjectArray<T>::Container ObjectArray<T>::Create(GLsizei n)
{
    static_assert(sizeof(T) == sizeof(GLuint) && alignof(T) == alignof(GLuint),
        "T must be of same size and alignment as GLuint. Do not add any member variables or virtual functions");
    if(n > 0) {
        Container result(n);
        T::CreateArray(n, &result[0].Access());
        return result;
    } else {
        return Container();
    }
}

template<class T>
ObjectArray<T>::ObjectArray() :
    Object(nullptr)
{
    T::CreateArray(1, &Access());
}

template<class T>
void ObjectArray<T>::Destroy(const GLuint& value)
{
    static_assert(std::is_base_of<ObjectArray<T>, T>::value, "Usage: class Texture : public ObjectArray<Texture>");
    T::DestroyArray(1, &value);
}

template<class T>
void ObjectArray<T>::Bind() const
{
    T::BindImpl(Get());
}

template<class T>
void ObjectArray<T>::BindDefault()
{
    T::BindImpl(0);
}

} // namespace GL
