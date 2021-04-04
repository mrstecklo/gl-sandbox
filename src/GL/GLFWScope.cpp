#include "GLFWScope.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace GLFW {

bool Scope::instantiated = false;

Scope::Scope()
{
    if(instantiated) {
        throw std::runtime_error("GL::FWScope instance already exist");
    }
    
    instantiated = (glfwInit() != GL_FALSE);

    if(!instantiated) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
}

Scope::~Scope()
{
    glfwTerminate();
    instantiated = false;
}


} // namespace GL