#include "../Header/VulkanImage.h"

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

VulkanImage::VulkanImage(VulkanDevice * device)
	: VulkanDeviceChild(device),
	image(VK_NULL_HANDLE),
	imageView(VK_NULL_HANDLE)
{
}

VulkanImage::VulkanImage(VkImage & image, VulkanDevice * device)
	: VulkanDeviceChild(device),
	image(image),
	imageView(VK_NULL_HANDLE)
{
}

 VulkanImage::~VulkanImage()
{ }

void VulkanImage::InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags)
{
	if (image == VK_NULL_HANDLE)
	{
		Console_Log("Warning : image�� �ʱ�ȭ ���� �ʾ����� InitWithoutImage�� ȣ��");
		return;
	}

	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	//MEMO : �̹����� Ÿ���� ���� �� �� ����
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;

	//MEMO : �Ӹ� �� ���� ���̾� ���� ����
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkResult result = vkCreateImageView(device->GetHandle(), &viewInfo, nullptr, &imageView);
	Check_Throw(result != VK_SUCCESS, "�̹��� �� ���� ����!");
}

 void VulkanImage::Shutdown()
{
	 vkDestroyImageView(device->GetHandle(), imageView, nullptr);
	 vkDestroyImage(device->GetHandle(), image, nullptr);
}

void VulkanImage::ShutdownWithoutImage()
{
	vkDestroyImageView(device->GetHandle(), imageView, nullptr);
}

void VulkanImage::CreateImage()
{
}

void VulkanImage::CreateImageView()
{
}
