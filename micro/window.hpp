#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow* WindowInstance;
public:
	Window(const char* Title, int Width = 800, int Height = 600);
	~Window();
};