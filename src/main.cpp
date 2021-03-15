#include <iostream>

#include "GLCPP.h"
#include "GLFWScope.h"
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

void GLAPIENTRY MessageCallback (
    GL::Debug::Source source,
    GL::Debug::Type type,
    GLuint id,
    GL::Debug::Severity severity,
    GLsizei size,
    const GLchar* message,
    const void* userParam)
{
	std::cout << GL::ToString(type) << ": " << GL::ToString(severity) << ": " << GL::ToString(source) << std::endl
		<< message << std::endl;
}

int main( void )
{
	try {
		std::cout << "Hello world!" << std::endl;
		// Initialise GLFW
		GLFW::Scope glfw;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow( 1024, 768, "GL Sandbox", NULL, NULL);
		if( window == NULL ){
			throw std::exception("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
		}
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			 throw std::exception("Failed to initialize GLEW");
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		GL::Debug::Enable();
		GL::Debug::RegisterCallback(MessageCallback, 0);

		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		GL::VertexArray arr;
		arr.Bind();

		const GLfloat g_vertex_buffer_data[] = { 
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f,  1.0f, 0.0f,
		};

		GL::ArrayBuffer vertexBuffer;
		vertexBuffer.Bind();
		GL::ArrayBuffer::Data(g_vertex_buffer_data, GL::STATIC_DRAW);

		do{

			// Clear the screen
			glClear( GL_COLOR_BUFFER_BIT );

			// Use our shader
			//glUseProgram(programID);

			// 1rst attribute buffer : vertices
			GL::VertexAttribArray vertices(0);

			vertexBuffer.Bind();
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 );

		//glDeleteProgram(programID);

		return 0;
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	} catch (...) {
		std::cout << "unknown exception" << std::endl;
		return -2;
	}
}

