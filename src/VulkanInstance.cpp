#include "VulkanInstance.hpp"

VulkanInstance::VulkanInstance()
{
	if (enableValidationLayers)
	{
		valLayers = std::make_unique<ValidationLayers>();
	}

	deviceManager = std::make_unique<DeviceManager>();
}

void VulkanInstance::initVulkan()
{
	createInstance();
	valLayers->setupDebugMessenger(instance);
	deviceManager->pickPhysicalDevice(instance);
}

void VulkanInstance::createInstance()
{
	if (enableValidationLayers && !valLayers->checkValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available");
	}

	// Optional app info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "My VK Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		valLayers->populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	//Throw error if failed to initilise instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance");
	}
}

std::vector<const char *> VulkanInstance::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void VulkanInstance::cleanup()
{
	valLayers->cleanup(instance);

	vkDestroyInstance(instance, nullptr);
}