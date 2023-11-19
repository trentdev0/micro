#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <vector>

#include "window.hpp"

int main(int argc, const char *argv[])
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	Window WindowInstance = Window("Micro", 800, 600);

	WindowInstance.RenderLoop();

	glfwTerminate();

	return 0;
}