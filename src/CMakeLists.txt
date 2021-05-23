cmake_minimum_required(VERSION 3.10)

set(GLEW_INCLUDE_DIR CACHE PATH "Path to the GL/glew.h")
set(GLFW_INCLUDE_DIR CACHE PATH "Path to the GLFW/glfw3.h")
set(GLM_INCLUDE_DIR CACHE PATH "Path to the glm/glm.hpp")

set(GLEW_LIB_DIR CACHE PATH "Path to the glew32s.lib")
set(GLFW_LIB_DIR CACHE PATH "Path to the glfw3.lib")

add_executable(GLApp
   main.cpp

   Image/Load.cpp

   GLCPP/Debug.cpp
   
   GLFW/Scope.cpp
   GLFW/Window.cpp

   GUI/MainWindow.cpp
   GUI/Object.cpp

   Tetris/Map.cpp
   Tetris/Scene.cpp
   Tetris/ResourceManager.cpp
)

target_include_directories(GLApp PUBLIC
   .
)

target_include_directories(GLApp SYSTEM PUBLIC
   ${GLEW_INCLUDE_DIR}
   ${GLFW_INCLUDE_DIR}
   ${GLM_INCLUDE_DIR}
)

if(MSVC)
   target_compile_options(GLApp PRIVATE
      /W4
      /wd4201
   )

   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

   target_link_options(GLApp PUBLIC
      /NODEFAULTLIB:LIBCMT
      /NODEFAULTLIB:LIBCMTD
      /NODEFAULTLIB:MSVCRT$<$<NOT:$<CONFIG:Debug>>:D>
   )
else()
   target_compile_options(GLApp PRIVATE
      -Wall
      -DGLM_ENABLE_EXPERIMENTAL
   )
endif()

add_definitions(
   -DGLEW_STATIC
)

find_library(GLFW3_LIB
   NAMES glfw3 glfw
   HINTS
      ${GLFW_LIB_DIR}
)

find_library(GLEW_LIB
   NAMES glew32s GLEW
   HINTS
      ${GLEW_LIB_DIR}
)

if(WIN32)

set(OPENGL_LIB opengl32)

else()

find_library(OPENGL_LIB OpenGL)

endif()

target_link_libraries(GLApp  
   ${OPENGL_LIB}
   ${GLFW3_LIB}
   ${GLEW_LIB}
)
