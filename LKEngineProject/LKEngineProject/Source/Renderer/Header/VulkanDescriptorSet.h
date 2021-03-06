#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanBuffer;
class VulkanTexture;
class VulkanDescriptorPool;
class VulkanDescriptorSetLayout;

class VulkanDescriptorSet
	: public VulkanDeviceChild
{
private:
	VkDescriptorSet descriptorSet;

	std::vector<VkDescriptorBufferInfo> bufferInfos;
	std::vector<VkDescriptorImageInfo> imageInfos;

	std::vector<VkWriteDescriptorSet> descriptorWrites;
public:
	explicit VulkanDescriptorSet(VulkanDescriptorSetLayout* setLayout, VulkanDescriptorPool* pool);

	void AddBufferInfo(VkDescriptorType type, VulkanBuffer* buffer, size_t offset, uint32_t binding);
	void AddTextureInfo(VkDescriptorType type, const VkImageView& imageView, const VkSampler& sampler , uint32_t binding);

	void UpdateSets();

	const VkDescriptorSet& GetHandle() const;
};

LK_VULKAN_SPACE_END