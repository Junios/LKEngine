#include "../Header/VulkanCommandBuffers.h"

#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanCommandPool.h"
#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

VulkanCommandBuffers::VulkanCommandBuffers(VulkanDevice * device, VulkanCommandPool* commandPool)
	:VulkanDeviceChild(device),
	commandPool(commandPool)
{
}

void VulkanCommandBuffers::Init(VulkanSwapchain * swapchain)
{
	Console_Log("��� ���� ���� ����");
	commandBuffers.resize(swapchain->GetImages().size());

	VkCommandBufferAllocateInfo allocInfo = { };
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool->GetHandle();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = commandBuffers.size();

	Check_Throw(vkAllocateCommandBuffers(device->GetHandle(), &allocInfo, commandBuffers.data()) != VK_SUCCESS, "Command Buffer ���� ����");
	Console_Log("��� ���� ���� ����");
}

void VulkanCommandBuffers::Shutdown()
{
}
