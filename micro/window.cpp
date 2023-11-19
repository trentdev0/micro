#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "window.hpp"
#include "debug.hpp"

Window::Window(const char *WindowName, int Width, int Height)
{
	this->WindowInstance = glfwCreateWindow(Width, Height, WindowName, nullptr, nullptr);

	if(!this->WindowInstance)
	{
		throw std::runtime_error("Failed to create a window!");
	}

    VkApplicationInfo AppInformation{};
    AppInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInformation.pApplicationName = "Micro";
    AppInformation.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInformation.pEngineName = "No Engine";
    AppInformation.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInformation.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pApplicationInfo = &AppInformation;

    auto Extensions = Debugging::GetRequiredExtensions();
    CreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
    CreateInfo.ppEnabledExtensionNames = Extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};
    if(Debugging::EnableValidationLayers)
    {
        CreateInfo.enabledLayerCount = static_cast<uint32_t>(Debugging::ValidationLayers.size());
        CreateInfo.ppEnabledLayerNames = Debugging::ValidationLayers.data();

        Debugging::PopulateDebugMessengerCreateInfo(DebugCreateInfo);
        CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugCreateInfo;
    }
    else
    {
        CreateInfo.enabledLayerCount = 0;
        CreateInfo.pNext = nullptr;
    }

    if(vkCreateInstance(&CreateInfo, nullptr, &this->VulkanInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }

    if (Debugging::EnableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT CreateInfo;
        Debugging::PopulateDebugMessengerCreateInfo(CreateInfo);

        if(Vulkan::CreateDebugUtilsMessengerEXT(this->VulkanInstance, &CreateInfo, nullptr, &this->DebugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }

    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(this->VulkanInstance, &DeviceCount, nullptr);
    if(DeviceCount == 0)
    {
        throw std::runtime_error("Failed to find any GPU that supports Vulkan!");
    }

    this->Devices.resize(DeviceCount);
    vkEnumeratePhysicalDevices(this->VulkanInstance, &DeviceCount, this->Devices.data());

    for(const auto& Device : this->Devices)
    {
        Graphics::QueueFamilyIndices Indices = Graphics::FindQueueFamilies(Device);

        if(Graphics::IsQueueFamilyIndicesComplete(Indices))
        {
            this->PrimaryDevice = Device;
            break;
        }
    }

    if(this->PrimaryDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

Window::~Window()
{
    if(Debugging::EnableValidationLayers)
    {
        Vulkan::DestroyDebugUtilsMessengerEXT(this->VulkanInstance, this->DebugMessenger, nullptr);
    }

    vkDestroyInstance(this->VulkanInstance, nullptr);
	glfwDestroyWindow(this->WindowInstance);
}

void Window::RenderLoop()
{
    while(!glfwWindowShouldClose(this->WindowInstance))
    {
        glfwPollEvents();
    }
}

Graphics::QueueFamilyIndices Graphics::FindQueueFamilies(VkPhysicalDevice Device)
{
    Graphics::QueueFamilyIndices Indices;

    uint32_t QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

    int i = 0;
    for(const auto& QueueFamily : QueueFamilies)
    {
        if(QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            Indices.GrapicsFamily = i;
        }

        if(Graphics::IsQueueFamilyIndicesComplete(Indices))
        {
            break;
        }

        i++;
    }

    return Indices;
}