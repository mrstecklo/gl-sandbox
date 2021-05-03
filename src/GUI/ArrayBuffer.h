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
    GLboolean IsNormalized() const { return norm; }

    ArrayBuffer() = default;
    ArrayBuffer(ArrayBuffer&&) = default;
    ArrayBuffer& operator=(ArrayBuffer&&) = default;

protected:
    void Init(GLsizei numElements, GLint elemSize, GLboolean normalized)
    {
        num = numElements;
        size = elemSize;
        norm = normalized;
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
    GLboolean       norm = true; 
};

template<class T>
class ArrayBufferTyped : public ArrayBuffer {
public:
    ArrayBufferTyped() = default;
    ArrayBufferTyped(GLsizei numElements, GLint elemSize, const T* data, GL::BufferUsage usage, GLboolean normalized = true)
    {
        Data(numElements, elemSize, data, usage, normalized);
    }

    template<GLsizei numElements, GLint elemSize>
    ArrayBufferTyped(const T (&data)[numElements][elemSize], GL::BufferUsage usage, GLboolean normalized = true)
    {
        Data(data, usage, normalized);
    }

    void Data(GLsizei numElements, GLint elemSize, const T* data, GL::BufferUsage usage, GLboolean normalized)
    {
        Init(numElements, elemSize, normalized);
        GL::ArrayBuffer::Data(sizeof(T) * numElements * elemSize, data, usage);
    }

    template<GLsizei numElements, GLint elemSize>
    void Data(const T (&data)[numElements][elemSize], GL::BufferUsage usage, GLboolean normalized)
    {
        Init(numElements, elemSize, normalized);
        GL::ArrayBuffer::Data(data, usage);
    }

private:
    void VertexAttribPointerImpl(GL::VertexAttrib index) const override
    {
        GL::VertexAttribPointer(index, GetElemSize(), GL::TypeTraits<T>::type, IsNormalized(), 0, nullptr);
    }
};

using ArrayBufferFloat = ArrayBufferTyped<float>;

} // namespace GUI
