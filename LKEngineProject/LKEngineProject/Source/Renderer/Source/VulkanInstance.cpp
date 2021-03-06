#include "../Header/VulkanInstance.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "../Header/VulkanExtension.h"
#include "../Header/VulkanDebug.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanInstance::VulkanInstance()
	:vkInstance(VK_NULL_HANDLE),
	debug(nullptr)
{ }

VulkanInstance::~VulkanInstance()
{
	SAFE_DELETE(debug);
}

void VulkanInstance::Init(bool vaildationLayerOn)
{
	VulkanExtension extension;

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Unnamed";
	appInfo.applicationVersion = APPLICATION_VERSION;
	appInfo.pEngineName = ENGINE_NAME;
	appInfo.engineVersion = LK_VERSION;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = extension.GetInstanceExtensions(vaildationLayerOn);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (vaildationLayerOn)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(vaildationLayers.size());
		createInfo.ppEnabledLayerNames = vaildationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	Check_Throw(VK_SUCCESS != vkCreateInstance(&createInfo, nullptr, &vkInstance), "VkInstance ���� ����!");

	if (vaildationLayerOn)
	{
		debug = new VulkanDebug(this);
		debug->Init();
	}
}

void VulkanInstance::Shutdown()
{
	debug->Shutdown();

	vkDestroyInstance(vkInstance, nullptr);
}

VkInstance VulkanInstance::GetHandle() const
{
	return vkInstance;
}
