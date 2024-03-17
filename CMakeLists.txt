cmake_minimum_required(VERSION 3.10)

project(WhimEngine)

set(CMAKE_CXX_STANDARD 17)

# Add all the source files
add_executable(WhimEngine "${PROJECT_SOURCE_DIR}/GLFWTest/GLFWTest.cpp")

# Add the include directories
include_directories(
    "C:/Libraries/glm"
    "C:/Libraries/glad/include"
    "C:/Libraries/glew-2.2.0/include"
    "C:/Libraries/glfw-3.3.8.bin.WIN64/include"
)


# TODO: fix the linking errors 
target_link_libraries(WhimEngine
    "C:/Libraries/glew-2.2.0/lib/Release/x64/glew32s.lib"
    "C:/Libraries/glfw-3.3.8.bin.WIN64/lib-vc2019/glfw3.lib"
    "C:/"
)