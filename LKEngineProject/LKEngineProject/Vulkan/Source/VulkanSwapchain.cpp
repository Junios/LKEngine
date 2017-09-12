#include "../Header/VulkanSwapchain.h"

using namespace LKEngine::Vulkan;

SwapchainSupportDetail::SwapchainSupportDetail(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
	//�����̽� ��� ����
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);

	//�����̽� ���� ����
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);
	if (formatCount != 0) 
	{
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, formats.data());
	}

	//�����̽� ������Ʈ ��� ����
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) 
	{
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, presentModes.data());
	}
}

bool SwapchainSupportDetail::CheckSwapchainAdequate()
{
	return !(formats.empty() || presentModes.empty());
}
