#include "../Header/VulkanSwapchain.h"

#include <GLFW/glfw3.h>

#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/WindowsWindow.h"
#include "../Header/VulkanQueueFamilyIndices.h"
#include "../Header/VulkanDevice.h"
#include "../Header/VulkanImage.h"

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

VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, LKEngine::Window::WindowsWindow* window)
	: VulkanDeviceChild(device),
	window(window),
	swapchain(VK_NULL_HANDLE)
{ }

VulkanSwapchain::~VulkanSwapchain()
{
	for (auto& image : swapchainImages)
	{
		SAFE_DELETE(image);
	}
}

void VulkanSwapchain::Init(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface)
{
	Console_Log("���� ü�� ���� ����");

	SwapchainSupportDetail swapchainSupport(gpu, surface);

	auto surfaceFormat = ChooseSurfaceFormat(swapchainSupport.formats);
	auto presentMode = ChoosePresentMode(swapchainSupport.presentModes);
	auto extent = ChooseSwapchainExtent(swapchainSupport.capabilities);

	uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
	if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount)
	{
		imageCount = swapchainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices;
	indices.FindQueueFamily(gpu, surface);
	uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily),static_cast<uint32_t>(indices.presentFamily) };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	//MEMO : ���� ü�� �̹����� ��ȯ ���� ����
	createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
	//MEMO : Windows���� �ٸ� ������� ȥ���ϱ� ���� ���� ä���� ����ϴ��� ���� ���� ����
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	VkResult result = vkCreateSwapchainKHR(device->GetRawDevice(), &createInfo, nullptr, &swapchain);
	Check_Throw(result != VK_SUCCESS, "���� ü�� ���� ����!");

	std::vector<VkImage> tmpImages;
	vkGetSwapchainImagesKHR(device->GetRawDevice(), swapchain, &imageCount, nullptr);
	tmpImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device->GetRawDevice(), swapchain, &imageCount, tmpImages.data());

	swapchainImages.resize(imageCount);
	for (size_t i = 0; i < swapchainImages.size(); i++)
	{
		VulkanImage* image = new VulkanImage(tmpImages[i], device);
		image->InitWithoutImage(surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);

		swapchainImages[i] = image;
	}

	swapchainFormat = surfaceFormat.format;
	swapchainExtent = extent;

	Console_Log("���� ü�� ���� ����");
}

void VulkanSwapchain::Shutdown()
{
	for (size_t i = 0; i < swapchainImages.size(); i++)
	{
		swapchainImages[i]->ShutdownWithoutImage();
	}

	vkDestroySwapchainKHR(device->GetRawDevice(), swapchain, nullptr);
}

VkFormat VulkanSwapchain::GetFormat() const
{
	return swapchainFormat;
}

VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
	//Best One
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) 
	{
		return VkSurfaceFormatKHR{ VK_FORMAT_B8G8R8A8_UNORM,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	//Others
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	//Worst One
	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentModes : availablePresentModes)
	{
		if (availablePresentModes == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentModes;
		}
		else if (availablePresentModes == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestMode = availablePresentModes;
		}
	}

	return bestMode;
}

VkExtent2D VulkanSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR & capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetWindowSize(window->GetWindowHandle(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}
