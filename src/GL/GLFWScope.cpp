#include "GLFWScope.h"
#include <GLFW/glfw3.h>
#include <exception>

namespace GLFW {

bool Scope::instantiated = false;

Scope::Scope()
{
    if(instantiated) {
        throw std::exception("GL::FWScope instance already exist");
    }
    
    instantiated = glfwInit();

    if(!instantiated) {
		throw std::exception("Failed to initialize GLFW");
	}
}

Scope::~Scope()
{
    glfwTerminate();
    instantiated = false;
}


} // namespace GL