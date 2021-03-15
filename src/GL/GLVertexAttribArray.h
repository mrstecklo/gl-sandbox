#pragma once 

#include <GL/glew.h>
#include "Noncopyable.h"

namespace GL {

class VertexAttribArray : Util::Noncopyable {
public:
    constexpr VertexAttribArray() = default;
    VertexAttribArray(GLuint i) :
        valid(true),
        index(i)
    {
        glEnableVertexAttribArray(index);
    }

    ~VertexAttribArray() { if(valid) { glDisableVertexAttribArray(index); } }

    VertexAttribArray(VertexAttribArray&& other) :
        valid(std::exchange(other.valid, false)),
        index(other.index)
    {}  
    
    VertexAttribArray& operator=(VertexAttribArray&& other)
    {
        std::swap(valid, other.valid);
        std::swap(index, other.index);
    }
private:
    bool valid = false;
    GLuint index = 0;
};

} // namespace GL