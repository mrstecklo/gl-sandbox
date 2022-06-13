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
