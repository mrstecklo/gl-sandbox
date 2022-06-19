cmake_minimum_required(VERSION 3.10)

set(GLM_INCLUDE_DIR CACHE PATH "Path to the glm/glm.hpp")

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
   Tetris/Visualisation.cpp
)

target_include_directories(GLApp PUBLIC
   .
)

target_include_directories(GLApp SYSTEM PUBLIC
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

if(WIN32)
   set(OPENGL_LIB opengl32)
else()
   find_library(OPENGL_LIB OpenGL)
endif()

target_link_libraries(GLApp  
   ${OPENGL_LIB}
   glfw
   glew
)
