#include "../Header/VulkanBuffer.h"

#include "../Header/VulkanQueue.h"
#include "../Header/VulkanCommandBuffers.h"
#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

VulkanBuffer::VulkanBuffer(VulkanDevice * device)
	:VulkanDeviceChild(device)
{
}

void VulkanBuffer::Init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode)
{
	VkBufferCreateInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.size = size;
	info.usage = usage;
	info.sharingMode = sharingMode;
	bufferSize = size;

	Check_Throw(vkCreateBuffer(device->GetHandle(), &info, nullptr, &buffer) != VK_SUCCESS, "���� ���� ����");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device->GetHandle(), buffer, &memRequirements);
	
	VkMemoryAllocateInfo allocInfo = { };
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, properties);
	Check_Throw(vkAllocateMemory(device->GetHandle(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS, "���� �޸� �Ҵ� ����");

	vkBindBufferMemory(device->GetHandle(), buffer, bufferMemory, 0);
}

void VulkanBuffer::Shutdown()
{
	vkDestroyBuffer(device->GetHandle(), buffer, nullptr);
	vkFreeMemory(device->GetHandle(), bufferMemory, nullptr);
}

const VkBuffer & VulkanBuffer::GetBuffer() const
{
	return buffer;
}

const VkDeviceMemory & VulkanBuffer::GetBufferMemory() const
{
	return bufferMemory;
}

const VkDeviceSize VulkanBuffer::GetBufferSize() const
{
	return bufferSize;
}

void VulkanBuffer::CopyBuffer(VulkanBuffer * dstBuffer, VulkanCommandPool * commandPool, VulkanQueue * transferQueue)
{
	//TODO : ���߿� �ӽ� ��� ���� ���� ��� Ǯ �����
	//�ӽ� ���Ǯ�� VK_COMMAND_POOL_CREATE_TRANSIMENT_BIT ����ؾ� ��
	VulkanCommandBuffers cmdBuffers(device, commandPool);
	cmdBuffers.AllocBuffers(1);

	VkBufferCopy copyRegion = {};
	copyRegion.size = bufferSize;

	cmdBuffers.RecordBegin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkCmdCopyBuffer(cmdBuffers.GetBuffer(0), buffer, dstBuffer->GetBuffer(), 1, &copyRegion);

	cmdBuffers.RecordEnd(0);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffers.GetBuffer(0);
	transferQueue->Submit(submitInfo);
	transferQueue->WaitIdle();

	cmdBuffers.FreeAll();
}