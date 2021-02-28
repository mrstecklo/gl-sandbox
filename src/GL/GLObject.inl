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
    if(IsValid()) {
        T::Destroy(Get());
    }
}

} // namespace GL