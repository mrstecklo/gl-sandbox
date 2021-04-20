#include "MainWindow.h"
#include "GLFWScope.h"
#include <iostream>

void GLAPIENTRY MessageCallback (
    GL::Debug::Source source,
    GL::Debug::Type type,
    GLuint id,
    GL::Debug::Severity severity,
    GLsizei size,
    const GLchar* message,
    const void* userParam)
{
	std::cout << ToString(type) << ": " << ToString(severity) << ": " << ToString(source) << std::endl
		<< message << std::endl;
}

int main( void )
{
	try {
		std::cout << "Hello world!" << std::endl;

		GLFW::Scope glfw;

		GUI::MainWindow window(
			glfw.CreateWindow()
			.Hint(GLFW::SAMPLES, 4)
			.Hint(GLFW::CONTEXT_VERSION_MAJOR, 3)
			.Hint(GLFW::CONTEXT_VERSION_MINOR, 3)
			.Hint(GLFW::OPENGL_FORWARD_COMPAT, true)
			.Hint(GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE)
			.Finalize(2048, 768, "GL Sandbox", nullptr, nullptr));

		if(!window.IsOpen()){
			throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
		}

		window.SetInputMode(GLFW::STICKY_KEYS, true);

		GL::Debug::Enable();
		GL::Debug::RegisterCallback(MessageCallback, 0);

		do {
			window.Render();
			glfw.PollEvents();
		} while( window.GetKey(GLFW_KEY_ESCAPE) != GLFW::PRESS && !window.ShouldClose());

		return 0;
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return -1;
	} catch (...) {
		std::cout << "Unknown exception" << std::endl;
		return -2;
	}
}
