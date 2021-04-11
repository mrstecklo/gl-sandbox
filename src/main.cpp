



#include "GLCPP.h"
#include "GLFWScope.h"


#include <iostream>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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
"layout(location = 1) in vec3 vertexColor;"
"out vec3 UV;"
"uniform mat4 MVP = {"
"{1.f, 0.f, 0.f, 0.f},"
"{0.f, 1.f, 0.f, 0.f},"
"{0.f, 0.f, 1.f, 0.f},"
"{0.f, 0.f, 0.f, 1.f}};"
"void main(){"
" gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
" UV = vertexColor;"
"}";

const GLchar fragmentShader[] = 
"#version 330 core\n"
"in vec3 UV;"
"out vec3 color;"
"uniform sampler2D myTextureSampler;"
"void main() {"
"color = UV;"
"}";

static constexpr int width = 1024;
static constexpr int height = 768;


int main( void )
{
	try {
		GLFWwindow* window = nullptr;
		std::cout << "Hello world!" << std::endl;
		// Initialise GLFW
		GLFW::Scope glfw;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow( width, height, "GL Sandbox", NULL, NULL);
		if( window == NULL ){
			throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
		}
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			 throw std::runtime_error("Failed to initialize GLEW");
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		GL::Debug::Enable();
		GL::Debug::RegisterCallback(MessageCallback, 0);

		// Dark blue background
		GL::ClearColor(0.0f, 0.0f, 0.3f, 0.0f);
		GL::EnableDepthTest();

		GL::VertexArray arr;
		arr.Bind();

		static constexpr GLint vertexSize = 3;
		static constexpr GLint colorSize = 3;
		static constexpr GLsizei numVertices = 36;

		const GLfloat g_vertex_buffer_data[numVertices][vertexSize] = { 
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};
			
		const GLfloat g_color_buffer_data[numVertices][colorSize] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,
			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,
			0.310f,  0.747f,  0.185f,
			0.597f,  0.770f,  0.761f,
			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,
			0.483f,  0.596f,  0.789f,
			0.559f,  0.861f,  0.639f,
			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,
			0.771f,  0.328f,  0.970f,
			0.406f,  0.615f,  0.116f,
			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,
			0.140f,  0.616f,  0.489f,
			0.997f,  0.513f,  0.064f,
			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,
			0.279f,  0.317f,  0.505f,
			0.167f,  0.620f,  0.077f,
			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,
			0.714f,  0.505f,  0.345f,
			0.783f,  0.290f,  0.734f,
			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,
			0.225f,  0.587f,  0.040f,
			0.517f,  0.713f,  0.338f,
			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,
			0.673f,  0.211f,  0.457f,
			0.820f,  0.883f,  0.371f,
			0.982f,  0.099f,  0.879f
		};

		GL::ArrayBuffer vertexBuffer;
		vertexBuffer.Bind();
		GL::ArrayBuffer::Data(g_vertex_buffer_data, GL::STATIC_DRAW);

		GL::ArrayBuffer colorBuffer;
		colorBuffer.Bind();
		GL::ArrayBuffer::Data(g_color_buffer_data, GL::STATIC_DRAW);

		GL::Texture2D texture;
		texture.Bind();
		GL::Texture2D::Image(0, GL::RGB8, 800, 600, reinterpret_cast<GL::Pixel::BGR<GLubyte>*>(0));
		GL::Texture2D::SetMinFilter(GL::LINEAR_MIPMAP_LINEAR);
		GL::Texture2D::SetMagFilter(GL::LINEAR);

		auto prog = GL::Program::Create(vertexShader, nullptr, fragmentShader);
		GL::UniformMatrix4f MatrixID(prog.GetUniformLocation("MVP"));


		static constexpr auto attrVertices = GL::VertexAttrib(0);
		static constexpr auto attrColor = GL::VertexAttrib(1);


		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
		
		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
		
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(4,3,-3), // Camera is at (4,3,3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
		
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 CubeMat = glm::mat4(1.0f);
		glm::mat4 TriangleMat = glm::translate(glm::vec3(2.f, 0.f, -1.f)) * glm::scale(glm::vec3(2.f, 2.f, 2.f));
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 VP = Projection * View; // Remember, matrix multiplication is the other way around

		glm::mat4 CubeMVP = VP * CubeMat;
		glm::mat4 TriangleMVP = VP * TriangleMat;

		do{

			// Clear the screen
			GL::Clear( GL::COLOR_BUFFER_BIT, GL::DEPTH_BUFFER_BIT );

			// Use our shader
			prog.Use();
			MatrixID.Set(&CubeMVP[0][0]);
			// 1rst attribute buffer : vertices
			GL::VertexAttribArray vertices(attrVertices);
			GL::VertexAttribArray colors(attrColor);

			vertexBuffer.Bind();
			GL::VertexAttribPointer(
				attrVertices,
				vertexSize,          // size
				GL::FLOAT,           // type
				0,                   // stride
				nullptr              // array buffer offset
			);

			colorBuffer.Bind();
			GL::VertexAttribPointer(
				attrColor,
				colorSize,          // size
				GL::FLOAT,           // type
				0,                   // stride
				nullptr              // array buffer offset
			);

			// Draw the triangle !
			GL::DrawArrays(GL::TRIANGLES, numVertices);

			MatrixID.Set(&TriangleMVP[0][0]);

			GL::DrawArrays(GL::TRIANGLES, 3);

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

