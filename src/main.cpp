#include "GLCPP.h"
#include "GLFWScope.h"
#include "Image.h"

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
"layout(location = 1) in vec2 vertexUV;"
"out vec2 UV;"
"uniform mat4 MVP = {"
"{1.f, 0.f, 0.f, 0.f},"
"{0.f, 1.f, 0.f, 0.f},"
"{0.f, 0.f, 1.f, 0.f},"
"{0.f, 0.f, 0.f, 1.f}};"
"uniform mat3x2 UVMat = {"
"{1.f, 0.f},"
"{0.f, 1.f},"
"{0.f, 0.f}};"
"void main(){"
" gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
" UV = UVMat * vec3(vertexUV, 1);"
"}";

const GLchar fragmentShader[] = 
"#version 330 core\n"
"in vec2 UV;"
"out vec3 color;"
"uniform sampler2D texSampler;"
"void main() {"
"color = texture( texSampler, UV ).rgb;;"
"}";

static constexpr int width = 1024;
static constexpr int height = 768;


int main( void )
{
	try {
		std::cout << "Hello world!" << std::endl;

		GLFW::Scope glfw;

		auto window = glfw.CreateWindow()
			.Hint(GLFW::SAMPLES, 4)
			.Hint(GLFW::CONTEXT_VERSION_MAJOR, 3)
			.Hint(GLFW::CONTEXT_VERSION_MINOR, 3)
			.Hint(GLFW::OPENGL_FORWARD_COMPAT, true)
			.Hint(GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE)
			.Finalize(width, height, "GL Sandbox", nullptr, nullptr);

		if(!window.IsOpen()){
			throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
		}

		window.MakeCurrent();

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			 throw std::runtime_error("Failed to initialize GLEW");
		}

		window.SetInputMode(GLFW::STICKY_KEYS, true);


		GL::Debug::Enable();
		GL::Debug::RegisterCallback(MessageCallback, 0);

		// Dark blue background
		GL::ClearColor(0.0f, 0.0f, 0.3f, 0.0f);
		GL::EnableDepthTest();

		GL::VertexArray arr;
		arr.Bind();

		static constexpr GLint vertexSize = 3;
		static constexpr GLint UVSize = 2;
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
			
		static const GLfloat g_uv_buffer_data[numVertices][UVSize] = {
			0.000059f, 1.0f-0.000004f,
			0.000103f, 1.0f-0.336048f,
			0.335973f, 1.0f-0.335903f,
			1.000023f, 1.0f-0.000013f,
			0.667979f, 1.0f-0.335851f,
			0.999958f, 1.0f-0.336064f,
			0.667979f, 1.0f-0.335851f,
			0.336024f, 1.0f-0.671877f,
			0.667969f, 1.0f-0.671889f,
			1.000023f, 1.0f-0.000013f,
			0.668104f, 1.0f-0.000013f,
			0.667979f, 1.0f-0.335851f,
			0.000059f, 1.0f-0.000004f,
			0.335973f, 1.0f-0.335903f,
			0.336098f, 1.0f-0.000071f,
			0.667979f, 1.0f-0.335851f,
			0.335973f, 1.0f-0.335903f,
			0.336024f, 1.0f-0.671877f,
			1.000004f, 1.0f-0.671847f,
			0.999958f, 1.0f-0.336064f,
			0.667979f, 1.0f-0.335851f,
			0.668104f, 1.0f-0.000013f,
			0.335973f, 1.0f-0.335903f,
			0.667979f, 1.0f-0.335851f,
			0.335973f, 1.0f-0.335903f,
			0.668104f, 1.0f-0.000013f,
			0.336098f, 1.0f-0.000071f,
			0.000103f, 1.0f-0.336048f,
			0.000004f, 1.0f-0.671870f,
			0.336024f, 1.0f-0.671877f,
			0.000103f, 1.0f-0.336048f,
			0.336024f, 1.0f-0.671877f,
			0.335973f, 1.0f-0.335903f,
			0.667969f, 1.0f-0.671889f,
			1.000004f, 1.0f-0.671847f,
			0.667979f, 1.0f-0.335851f
		};

		GL::ArrayBuffer vertexBuffer;
		vertexBuffer.Bind();
		GL::ArrayBuffer::Data(g_vertex_buffer_data, GL::STATIC_DRAW);

		GL::ArrayBuffer UVBuffer;
		UVBuffer.Bind();
		GL::ArrayBuffer::Data(g_uv_buffer_data, GL::STATIC_DRAW);

		uint32_t mipmapCount = 1;
		bool invertV = false;
		GL::Texture2D texture = Image::Load("cube", &mipmapCount, &invertV);
		GL::Texture2D::SetMinFilter(GL::LINEAR_MIPMAP_LINEAR);
		GL::Texture2D::SetMagFilter(GL::LINEAR);
		if(mipmapCount == 1) {
			std::cout << "Texture file has no mipmaps" << std::endl;
			GL::Texture2D::GenerateMipmap();
		}

		auto prog = GL::Program::Create(vertexShader, nullptr, fragmentShader);
		GL::UniformMatrix4f MatrixID(prog.GetUniformLocation("MVP"));

		if(invertV) {
			GL::UniformMatrix3x2f UVMat(prog.GetUniformLocation("UVMat"));
			static const float ivertedUVMat[3][2] = {
				{1.f,  0.f},
				{0.f, -1.f},
				{0.f,  1.f}
			};
			prog.Use();
			UVMat.Set(ivertedUVMat);
		}


		static constexpr auto attrVertices = GL::VertexAttrib(0);
		static constexpr auto attrUV = GL::VertexAttrib(1);


		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.f);
		
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
			GL::VertexAttribArray UVs(attrUV);

			vertexBuffer.Bind();
			GL::VertexAttribPointer(
				attrVertices,
				vertexSize,          // size
				GL::FLOAT,           // type
				0,                   // stride
				nullptr              // array buffer offset
			);

			UVBuffer.Bind();
			GL::VertexAttribPointer(
				attrUV,
				UVSize,          // size
				GL::FLOAT,           // type
				0,                   // stride
				nullptr              // array buffer offset
			);

			// Draw the triangle !
			GL::DrawArrays(GL::TRIANGLES, numVertices);

			MatrixID.Set(&TriangleMVP[0][0]);

			GL::DrawArrays(GL::TRIANGLES, 3);

			window.SwapBuffers();
			glfw.PollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while( window.GetKey(GLFW_KEY_ESCAPE) != GLFW::PRESS && !window.ShouldClose());

		return 0;
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return -1;
	} catch (...) {
		std::cout << "Unknown exception" << std::endl;
		return -2;
	}
}
