cmake_minimum_required(VERSION 3.12)
project(micro)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "micro/*.cpp")

add_executable(micro ${SOURCES})

target_include_directories(micro PUBLIC ${CMAKE_SOURCE_DIR}/micro)

include(FindVulkan)
if(NOT VULKAN_FOUND)
    message(FATAL_ERROR "Vulkan SDK not installed.")
endif()

add_subdirectory(glfw)
add_subdirectory(glm)

target_compile_features(micro PRIVATE cxx_std_17)

target_link_libraries(micro glfw glm Vulkan::Vulkan ${GLFW_LIBRARIES})