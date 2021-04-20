#include "GLFWScope.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace GLFW {

std::atomic<bool> Scope::instantiated(false);

Scope::Scope()
{
    bool expected = false;
    if(instantiated.compare_exchange_strong(expected, true)) {
        if(glfwInit() == GL_FALSE) {
            instantiated = false;
            throw std::runtime_error("Failed to initialize GLFW");
        }
    } else {
        throw std::runtime_error("GL::FWScope instance already exist");
    }
}

Scope::~Scope()
{
    glfwTerminate();
    instantiated = false;
}


} // namespace GL
