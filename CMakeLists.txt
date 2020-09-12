cmake_minimum_required(VERSION 3.17)
project(OpenGLShare)

set(CMAKE_CXX_STANDARD 14)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

get_cmake_property(_variableNames VARIABLES)
list(SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach ()

set(ALL_LIBS ${OPENGL_LIBRARIES}
        glfw
        glm::glm
        GLEW::glew)

add_executable(first_triangle
        first_triangle/main.cpp
        shader.cpp
        )

target_link_libraries(
        first_triangle
        ${ALL_LIBS}
)
add_executable(colored_cube
        colored_cube/main.cpp
        shader.cpp
        )

target_link_libraries(
        colored_cube
        ${ALL_LIBS}
)

add_executable(textured_cube
        textured_cube/main.cpp
        textured_cube/texture.cpp
        shader.cpp
        )

target_link_libraries(
        textured_cube
        ${ALL_LIBS}
)