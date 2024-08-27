cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXTENSIONS OFF)
project(HelloWorld)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(fmt CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

target_link_libraries(
  ${PROJECT_NAME}
    PRIVATE fmt::fmt
    PRIVATE OpenGL::GL
    PRIVATE glfw
    PRIVATE glm::glm
    PRIVATE ${Vulkan_LIBRARIES}
)
