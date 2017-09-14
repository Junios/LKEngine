#pragma once

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"
#include <vulkan/vulkan.hpp>

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanImage
			: public BaseInterface,VulkanDeviceChild
		{
		private:
			VkImage image;
			VkImageView imageView;
		public:
			explicit VulkanImage(VulkanDevice* device);
			explicit VulkanImage(VkImage& image, VulkanDevice* device);
			virtual ~VulkanImage();

			//VkImageView�� �ʱ�ȭ(���� ü�ο�)
			void InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags);

			virtual void Shutdown() override;
			//VkImageView�� ����(���� ü�ο�)
			void ShutdownWithoutImage();

		private:
			void CreateImage();
			void CreateImageView();
		};
	}
}