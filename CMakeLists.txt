cmake_minimum_required(VERSION 3.10)

project(3DEngine)

# C++ VERSION
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# directories of sources and headers
include_directories(src objLoader)

# adding source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# creating an executable file
add_executable(3DEngine ${SOURCES})

set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

# Копирование шейдеров в build директорию
add_custom_command(TARGET 3DEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_DIR} $<TARGET_FILE_DIR:3DEngine>/shaders
)

# Копирование других ресурсов в build директорию
add_custom_command(TARGET 3DEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCES_DIR} $<TARGET_FILE_DIR:3DEngine>/resources
)

# including libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# linking libraries
target_link_libraries(3DEngine OpenGL::GL GLEW::GLEW glfw glm)
