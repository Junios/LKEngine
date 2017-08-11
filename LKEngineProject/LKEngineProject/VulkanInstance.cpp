#include "VulkanInstance.h"

#include <iostream>

#include "VulkanExtension.h"
#include "Macro.h"
#include "VulkanDebug.h"

void LKEngine::Vulkan::VulkanInstance::Init(bool vaildationLayerOn)
{
	extension = new VulkanExtension();
	extension->Init(vaildationLayerOn);

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

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extension->GetExtensions().size());
	std::vector<const char*> extensions = extension->GetExtensions();
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
		debug = new VulkanDebug();
		debug->Init(this);
	}
}

void LKEngine::Vulkan::VulkanInstance::CleanUp()
{
	delete extension;

	debug->CleanUp();
	delete debug;
	debug = nullptr;

	vkDestroyInstance(vkInstance, nullptr);
}

VkInstance LKEngine::Vulkan::VulkanInstance::GetRawInstance() const
{
	return vkInstance;
}
