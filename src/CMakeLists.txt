cmake_minimum_required(VERSION 3.10)

set(GLEW_INCLUDE_DIR CACHE PATH "Path to the GL/glew.h")
set(GLFW_INCLUDE_DIR CACHE PATH "Path to the GLFW/glfw3.h")
set(GLM_INCLUDE_DIR CACHE PATH "Path to the glm/glm.hpp")

set(GLEW_LIB_DIR CACHE PATH "Path to the glew32s.lib")
set(GLFW_LIB_DIR CACHE PATH "Path to the glfw3.lib")

add_executable(GLApp
   main.cpp

   Image/Image.cpp

   GL/GLDebug.cpp
   
   GLFW/GLFWScope.cpp
   GLFW/GLFWWindow.cpp

   GUI/MainWindow.cpp
   GUI/Object.cpp
)

target_include_directories(GLApp PUBLIC
   GL
   GLFW
   GUI
   Image
   Util
)

target_include_directories(GLApp SYSTEM PUBLIC
   ${GLEW_INCLUDE_DIR}
   ${GLFW_INCLUDE_DIR}
   ${GLM_INCLUDE_DIR}
)

if(MSVC)
   target_compile_options(GLApp PRIVATE /W4)

   target_link_options(GLApp PUBLIC
      /NODEFAULTLIB:LIBCMT
   )
else()
   target_compile_options(GLApp PRIVATE -Wall)
endif()

add_definitions(
   -DGLEW_STATIC
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
