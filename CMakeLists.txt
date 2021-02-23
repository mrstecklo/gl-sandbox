cmake_minimum_required(VERSION 3.10)

project(GLSandbox)

set(GLEW_INCLUDE_DIR CACHE PATH "Path to the GL/glew.h")
set(GLFW_INCLUDE_DIR CACHE PATH "Path to the GLFW/glfw3.h")

set(GLEW_LIB_DIR CACHE PATH "Path to the glew32s.lib")
set(GLFW_LIB_DIR CACHE PATH "Path to the glfw3.lib")

add_executable(GLApp src/main.cpp)

target_include_directories(GLApp PUBLIC
   ${GLEW_INCLUDE_DIR}
   ${GLFW_INCLUDE_DIR}
)

add_definitions(
   -DGLEW_STATIC
)

target_link_options(GLApp PUBLIC
   /NODEFAULTLIB:LIBCMT
)

target_link_directories(GLApp PUBLIC
   ${GLEW_LIB_DIR}
   ${GLFW_LIB_DIR}
)

target_link_libraries(GLApp
   opengl32
   glfw3
   glew32s
)
