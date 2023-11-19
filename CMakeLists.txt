cmake_minimum_required(VERSION 3.12)
project(micro)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "micro/*.cpp")

add_executable(micro ${SOURCES})

target_include_directories(micro PRIVATE ${CMAKE_SOURCE_DIR}/source)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(vkheaders)

target_link_libraries(micro glfw glm Vulkan::Headers ${GLFW_LIBRARIES})