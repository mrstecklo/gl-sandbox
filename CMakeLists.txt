cmake_minimum_required(VERSION 3.10)

project(GLSandbox)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/RUNTIME)

set(RUNTIME_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
if(MSVC)
    set(RUNTIME_DIRECTORY ${RUNTIME_DIRECTORY}/$(Configuration))
endif()


add_custom_target(resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${RUNTIME_DIRECTORY}/resources
)

add_subdirectory(external)
add_subdirectory(src)
