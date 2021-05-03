#pragma once

#include "GLCPP/Buffer.h"
#include "GLCPP/Draw.h"

namespace GUI {

class ArrayBuffer {
public:
    void Bind() const { buffer.Bind(); }
    void VertexAttribPointer(GL::VertexAttrib index) const { VertexAttribPointerImpl(index); }
    void DrawArrays(GL::DrawArraysMode mode) const { GL::DrawArrays(mode, num); }

    virtual ~ArrayBuffer() = default;

    GLint GetElemSize() const { return size; }
    GLsizei GetNumElements() const { return num; }

    ArrayBuffer() = default;
    ArrayBuffer(ArrayBuffer&&) = default;
    ArrayBuffer& operator=(ArrayBuffer&&) = default;

protected:
    void Init(GLsizei numElements, GLint elemSize)
    {
        num = numElements;
        size = elemSize;
        if(!buffer.IsValid()) {
            buffer = GL::ArrayBuffer();
        }
        buffer.Bind();
    }

private:
    virtual void VertexAttribPointerImpl(GL::VertexAttrib index) const = 0;

    GL::ArrayBuffer buffer {nullptr};
    
    GLsizei         num = 0;
    GLint           size = 0;
};

template<class T, GLboolean normalized = true>
class ArrayBufferTyped : public ArrayBuffer {
public:
    ArrayBufferTyped() = default;
    ArrayBufferTyped(GLsizei numElements, GLint elemSize, const T* data, GL::BufferUsage usage)
    {
        Data(numElements, elemSize, data, usage);
    }

    template<GLsizei numElements, GLint elemSize>
    ArrayBufferTyped(const T (&data)[numElements][elemSize], GL::BufferUsage usage)
    {
        Data(data, usage);
    }

    void Data(GLsizei numElements, GLint elemSize, const T* data, GL::BufferUsage usage)
    {
        Init(numElements, elemSize);
        GL::ArrayBuffer::Data(sizeof(T) * numElements * elemSize, data, usage);
    }

    template<GLsizei numElements, GLint elemSize>
    void Data(const T (&data)[numElements][elemSize], GL::BufferUsage usage)
    {
        Init(numElements, elemSize);
        GL::ArrayBuffer::Data(data, usage);
    }

private:
    void VertexAttribPointerImpl(GL::VertexAttrib index) const override
    {
        GL::VertexAttribPointer(index, GetElemSize(), GL::TypeTraits<T>::type, normalized, 0, nullptr);
    }
};

using ArrayBufferFloat = ArrayBufferTyped<float>;

} // namespace GUI
