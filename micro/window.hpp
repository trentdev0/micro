#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

class Window
{
	GLFWwindow *WindowInstance;
	VkInstance VulkanInstance;
	VkDebugUtilsMessengerEXT DebugMessenger;
public:
	std::vector<VkPhysicalDevice> Devices;
	VkPhysicalDevice PrimaryDevice;

	Window(const char* WindowName, int Width = 800, int Height = 600);
	~Window();
	void RenderLoop();
};

namespace Graphics
{
	typedef struct
	{
		std::optional<uint32_t> GrapicsFamily;
	} QueueFamilyIndices;

	static inline bool IsQueueFamilyIndicesComplete(QueueFamilyIndices &QueueFamilyIndicesInstance)
	{
		return QueueFamilyIndicesInstance.GrapicsFamily.has_value();
	}

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice Device);
};