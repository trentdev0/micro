#include <stdexcept>

#include "window.hpp"

Window::Window(const char *Title, int Width, int Height)
{
	this->WindowInstance = glfwCreateWindow(Width, Height, Title, NULL, NULL);
	if(!this->WindowInstance)
	{
		throw std::runtime_error("Failed to create a window!");
	}
}

Window::~Window()
{
	glfwDestroyWindow(this->WindowInstance);
}