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
	std::cout << ToString(type) << ": " << ToString(severity) << ": " << ToString(source) << std::endl
		<< message << std::endl;
}

const GLchar vertexShader[] =
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;"
"layout(location = 1) in vec2 vertexUV;"
"out vec2 UV;"
"uniform mat4 MVP = {"
"{1.f, 0.f, 0.f, 0.f},"
"{0.f, 1.f, 0.f, 0.f},"
"{0.f, 0.f, 1.f, 0.f},"
"{0.f, 0.f, 0.f, 1.f}};"
"void main(){"
" gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
" UV = vertexUV;"
"}";

const GLchar fragmentShader[] = 
"#version 330 core\n"
"in vec2 UV;"
"out vec3 color;"
"uniform sampler2D myTextureSampler;"
"void main() {"
"color = vec3(1, 0, 0);"
"}";


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

		static constexpr GLint vertexSize = 3;
		static constexpr GLsizei numVertices = 6;

		const GLfloat g_vertex_buffer_data[numVertices][vertexSize] = { 
			{-1.0f, -1.0f, 0.0f},
			{0.0f,  0.0f, 0.0f},
			{0.0f,  1.0f, 0.0f},
			{1.0f, -1.0f, 0.0f},
			{0.0f,  0.0f, 0.0f},
			{0.0f,  1.0f, 0.0f},
		};

		GL::ArrayBuffer vertexBuffer;
		vertexBuffer.Bind();
		GL::ArrayBuffer::Data(g_vertex_buffer_data, GL::STATIC_DRAW);

		auto prog = GL::Program::Create(vertexShader, nullptr, fragmentShader);

		static const auto attrVertices = GL::VertexAttrib(0);

		do{

			// Clear the screen
			glClear( GL_COLOR_BUFFER_BIT );

			// Use our shader
			prog.Use();

			// 1rst attribute buffer : vertices
			GL::VertexAttribArray vertices(attrVertices);

			vertexBuffer.Bind();
			GL::VertexAttribPointer(
				attrVertices,
				vertexSize,          // size
				GL::FLOAT,           // type
				0,                   // stride
				nullptr              // array buffer offset
			);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, numVertices); // 3 indices starting at 0 -> 1 triangle

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 );

		return 0;
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return -1;
	} catch (...) {
		std::cout << "Unknown exception" << std::endl;
		return -2;
	}
}

