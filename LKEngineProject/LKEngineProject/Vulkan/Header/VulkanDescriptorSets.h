#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanDescriptorSetLayout
		: public IVulkanObject, public VulkanDeviceChild
	{
	private:
		VkDescriptorSetLayout setLayout;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	public:
		explicit VulkanDescriptorSetLayout(VulkanDevice* device);

		virtual void Init() override { }
		virtual void Shutdown() override;

		void AddDescriptor(VkDescriptorType type, VkShaderStageFlagBits stageFlag, int index);
		void CreateDescriptorSetLayout();

		const VkDescriptorSetLayout& GetHandle() const;
	};
}