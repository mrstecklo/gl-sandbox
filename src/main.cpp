#include "GLCPP/Debug.h"
#include "Tetris/Scene.h"
#include "GUI/MainWindow.h"
#include "GLFW/Scope.h"

#include <iostream>

void GLAPIENTRY MessageCallback (
    GL::Debug::Source source,
    GL::Debug::Type type,
    GLuint /* id */,
    GL::Debug::Severity severity,
    GLsizei /* size */,
    const GLchar* message,
    const void* /* userParam */)
{
	std::cout << ToString(type) << ": " << ToString(severity) << ": " << ToString(source) << std::endl
		<< message << std::endl;
}

int main( void )
{
	try {
		GLFW::Scope glfw;

		GUI::MainWindow window(
			glfw.CreateWindow()
			.Hint(GLFW::SAMPLES, GLFW::Int(4))
			.Hint(GLFW::CONTEXT_VERSION_MAJOR, 4)
			.Hint(GLFW::CONTEXT_VERSION_MINOR, 2)
			.Hint(GLFW::OPENGL_FORWARD_COMPAT, true)
			.Hint(GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE)
			.Finalize(1024, 768, "GL Sandbox", nullptr, nullptr),
			std::make_unique<Tetris::Scene>()
		);

		if(!window.IsOpen()){
			throw std::runtime_error("Failed to open GLFW window");
		}

		//window.SetInputMode(GLFW::STICKY_KEYS, true);

		GL::Debug::Enable();
		GL::Debug::RegisterCallback(MessageCallback, 0);

		do {
			window.Render();
			glfw.PollEvents();
		} while(!window.ShouldClose());

		
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return -1;
	} catch (...) {
		std::cout << "Unknown exception" << std::endl;
		return -2;
	}

	std::cout << "Application finished successfully" << std::endl;
	return 0;
}
