#include "../Header/VulkanCommandBuffers.h"

#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanCommandPool.h"
#include "../Header/VulkanRenderPass.h"
#include "../Header/VulkanPipeline.h"
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

void VulkanCommandBuffers::Free()
{
	vkFreeCommandBuffers(device->GetHandle(), commandPool->GetHandle(), commandBuffers.size(), commandBuffers.data());
}

void VulkanCommandBuffers::Record(VulkanSwapchain * swapchain, VulkanRenderPass * renderPass, VulkanGraphicsPipeline * graphicsPipeline, std::vector<VkClearValue> clearColor)
{
	std::vector<VkFramebuffer> frameBuffers = swapchain->GetFrameBuffers();
	for (size_t i = 0 ; i < commandBuffers.size() ; i++)
	{
		VkCommandBufferBeginInfo info = { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		vkBeginCommandBuffer(commandBuffers[i], &info);

		renderPass->Begin(clearColor, frameBuffers[i], swapchain->GetExtent(), commandBuffers[i]);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetHandle());
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		renderPass->End(commandBuffers[i]);

		Check_Throw(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS, "��� ���� ��� ����");
	}
}

const VkCommandBuffer & VulkanCommandBuffers::GetBuffer(uint32_t index) const
{
	return commandBuffers[index];
}
