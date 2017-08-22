#include "VulkanDevice.h"

#include <vector>

#include "Macro.h"

LKEngine::Vulkan::VulkanDevice::VulkanDevice()
	:instance(nullptr),
	vkDevice(VK_NULL_HANDLE),
	gpu(VK_NULL_HANDLE)
{
	instance = new VulkanInstance(true);
}

LKEngine::Vulkan::VulkanDevice::~VulkanDevice()
{
	delete instance;
	instance = nullptr;
}

VkDevice LKEngine::Vulkan::VulkanDevice::GetRawDevice() const
{
	return vkDevice;
}

void LKEngine::Vulkan::VulkanDevice::RequirePhysicalDevice()
{
	gpu = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetRawInstance(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetRawInstance(), &deviceCount, devices.data());

	//TODO : ���Ŀ� ��� ������ GPU�� �ټ� �ϰ�� ���߿��� ���� ������ GPU�� �����ϵ��� ����
	for (auto device : devices)
	{
		if (CheckDeviceFeatures(device))
		{
			gpu = device;
			break;
		}
	}
	Check_Throw(gpu == VK_NULL_HANDLE, "��� ������ �׷��� ī�尡 �����ϴ�!");
}

bool LKEngine::Vulkan::VulkanDevice::CheckDeviceFeatures(VkPhysicalDevice device)
{
	return false;
}
