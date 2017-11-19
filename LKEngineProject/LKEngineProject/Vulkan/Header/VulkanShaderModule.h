#pragma once

#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanShaderModule
		: public VulkanDeviceChild
	{
	public:
		enum class ShaderType
		{
			VERTEX,
			FRAGMENT
		};

	private:
		ShaderType shaderType;

		VkShaderModule shaderModule;
	public:
		VulkanShaderModule(VulkanDevice* device);
		~VulkanShaderModule();

		void Init(const ShaderType type, const std::string& shaderPath);
		void Init(const ShaderType type, const std::vector<char>& compileCode);

		virtual void Shutdown();

		std::vector<char> ReadCompiledShader(const std::string& file);

		VkShaderModule GetHandle() const;
		ShaderType GetShaderType() const;
	};
}