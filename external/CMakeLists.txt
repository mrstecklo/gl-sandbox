include(ExternalProject)

function(add_external_library NAME)
    cmake_parse_arguments(EXTERNAL "" "" "LIBRARIES" ${ARGN})

    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${NAME})
    set(INSTALL_DIR ${PREFIX}/install)

    set(BUILD_BYPRODUCTS)
    foreach(LIBRARY ${EXTERNAL_LIBRARIES})
        list(APPEND BUILD_BYPRODUCTS ${INSTALL_DIR}/${LIBRARY})
    endforeach()

    set(INTERNAL_TARGET ${NAME}.internal)
    ExternalProject_Add(${INTERNAL_TARGET}
        ${EXTERNAL_UNPARSED_ARGUMENTS}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        PREFIX ${PREFIX}
        INSTALL_DIR ${INSTALL_DIR}
        BUILD_BYPRODUCTS ${BUILD_BYPRODUCTS}
    )

    add_library(${NAME} INTERFACE)
    add_dependencies(${NAME} ${INTERNAL_TARGET})
    target_link_libraries(${NAME} INTERFACE ${BUILD_BYPRODUCTS})
    target_include_directories(${NAME} INTERFACE ${INSTALL_DIR}/include)
endfunction()

function(add_binary_library NAME)
    cmake_parse_arguments(EXTERNAL "" "" "LIBRARIES" ${ARGN})

    set(INTERNAL_TARGET ${NAME}.internal)
    ExternalProject_Add(${INTERNAL_TARGET}
        ${EXTERNAL_UNPARSED_ARGUMENTS}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
    ExternalProject_Get_property(${INTERNAL_TARGET} SOURCE_DIR)

    add_library(${NAME} INTERFACE)
    add_dependencies(${NAME} ${INTERNAL_TARGET})

    foreach(LIBRARY ${EXTERNAL_LIBRARIES})
        target_link_libraries(${NAME} INTERFACE ${SOURCE_DIR}/${LIBRARY})
    endforeach()

    target_include_directories(${NAME} INTERFACE ${SOURCE_DIR}/include)
endfunction()

if(MSVC)
    set(GLFW_LIB_NAME glfw3)
else()
    set(GLFW_LIB_NAME glfw)
endif()

set(GLFW_LIB ${CMAKE_STATIC_LIBRARY_PREFIX}${GLFW_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})

add_external_library(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
    LIBRARIES lib/${GLFW_LIB}
)

if(MSVC)
    add_binary_library(glew
        URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip
        URL_MD5 1feddfe8696c192fa46a0df8eac7d4bf
        LIBRARIES lib/Release/x64/glew32s.lib
    )
    target_compile_definitions(glew INTERFACE -DGLEW_STATIC)
endif()
