#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "window.hpp"

std::vector<VkPhysicalDevice> PhysicalGraphicsDevices;
std::vector<VkQueueFamilyProperties> VulkanQueueFamilyProperties;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	Window MyWindow = Window("Micro", 800, 600);

	uint32_t NumberOfInstanceExtensions = 0;
	const char** ReqInstanceExtensions = glfwGetRequiredInstanceExtensions(&NumberOfInstanceExtensions);
	if(!ReqInstanceExtensions)
	{
		std::cerr << "Couldn't find any Vulkan extensions..." << std::endl;
		return -1;
	}

	VkApplicationInfo VulkanApplicationInformation = {};
	VulkanApplicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanApplicationInformation.pApplicationName = "Micro";
	VulkanApplicationInformation.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanApplicationInformation.pEngineName = "No Engine";
	VulkanApplicationInformation.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanApplicationInformation.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo VulkanInstanceCreateInformation = {};
	VulkanInstanceCreateInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	VulkanInstanceCreateInformation.pApplicationInfo = &VulkanApplicationInformation;
	VulkanInstanceCreateInformation.enabledExtensionCount = NumberOfInstanceExtensions;
	VulkanInstanceCreateInformation.ppEnabledExtensionNames = ReqInstanceExtensions;

	VkInstance VulkanInstance;
	VkResult Result = vkCreateInstance(&VulkanInstanceCreateInformation, NULL, &VulkanInstance);
	if(Result != VK_SUCCESS)
	{
		std::cerr << "Failed to create a Vulkan instsance!" << std::endl;
		return -2;
	}

	uint32_t NumberOfGPUs = 0;
	Result = vkEnumeratePhysicalDevices(VulkanInstance, &NumberOfGPUs, NULL);
	if(NumberOfGPUs <= 0)
	{
		std::cerr << "No GPUs were found!" << std::endl;
		return -3;
	}

	PhysicalGraphicsDevices.resize(NumberOfGPUs);

	for(int i = 0; i < NumberOfGPUs; i++)
	{
		Result = vkEnumeratePhysicalDevices(VulkanInstance, &NumberOfGPUs, &PhysicalGraphicsDevices.data()[i]);
		if(Result != VK_SUCCESS)
		{
			std::cerr << "Failed to enumerate GPU " << i << ", got result " << Result << "..." << std::endl;
			return -4;
		}
	}

	uint32_t NumberOfQueues = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalGraphicsDevices[0], &NumberOfQueues, NULL);
	if(NumberOfQueues <= 0)
	{
		std::cerr << "Failed to find any queue families!" << std::endl;
		return -5;
	}

	VulkanQueueFamilyProperties.resize(NumberOfQueues);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalGraphicsDevices[0], &NumberOfQueues, VulkanQueueFamilyProperties.data());

	int QueueIndex = -1;
	for(int i = 0; i < NumberOfQueues; i++)
	{
		if(VulkanQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			QueueIndex = i;
			break;
		}
	}

	if(QueueIndex < 0)
	{
		fprintf(stderr, "Could not find a queue family with graphics support\n");
		return -6;
	}

	if(!glfwGetPhysicalDevicePresentationSupport(VulkanInstance, PhysicalGraphicsDevices[0], QueueIndex))
	{
		std::cerr << "The selected queue family does not support present mode!" << std::endl;
		return -7;
	}

	uint32_t NumberOfDeviceExtensions = 0;
	Result = vkEnumerateDeviceExtensionProperties(PhysicalGraphicsDevices[0], NULL, &NumberOfDeviceExtensions, NULL);
	if(NumberOfDeviceExtensions <= 0 || Result != VK_SUCCESS)
	{
		std::cerr << "Could not find any Vulkan device extensions!" << std::endl;
		return -8;
	}

	/* To be continued... */

	std::cout << "Press any key to continue...";
	std::cin.get();

	return 0;
}