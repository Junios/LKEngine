#include "../Header/VulkanDevice.h"

#include <vector>
#include <set>

#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/Window.h"
#include "../Header/VulkanQueue.h"
#include "../Header/VulkanQueueFamilyIndices.h"
#include "../Header/VulkanExtension.h"
#include "../Header/VulkanSwapchain.h"

using namespace LKEngine::Vulkan;

VulkanDevice::VulkanDevice()
	:instance(nullptr),
	vkDevice(VK_NULL_HANDLE),
	gpu(VK_NULL_HANDLE)
{
	instance = new VulkanInstance();
}

VulkanDevice::~VulkanDevice()
{
	SAFE_DELETE(instance);
}

void VulkanDevice::Init(LKEngine::Window::Window* window, bool debug)
{
	instance->Init(debug);

	CreateSurface(window);

	RequirePhysicalDevice();

	CreateDevice(debug);
}

void VulkanDevice::Shutdown()
{
	instance->Shutdown();

	vkDestroySurfaceKHR(instance->GetRawInstance(), surface, nullptr);
}

VkDevice VulkanDevice::GetRawDevice() const
{
	return vkDevice;
}

void VulkanDevice::RequirePhysicalDevice()
{
	Console_Log("�׷��� ī�� ����");
	gpu = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetRawInstance(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetRawInstance(), &deviceCount, devices.data());

	//TODO : ���Ŀ� ��� ������ GPU�� �ټ� �ϰ�� ���߿��� ���� ������ GPU�� �����ϵ��� ����
	for (auto device : devices)
	{
		if (CheckDeviceFeatures(device))
		{
			gpu = device;
			break;
		}
	}

	Check_Throw(gpu == VK_NULL_HANDLE, "��� ������ �׷��� ī�尡 �����ϴ�!");
	Console_Log("�׷��� ī�� ���� �Ϸ�!");

	vkGetPhysicalDeviceProperties(gpu, &gpuProp);
	auto GetDeviceTypeString = [&]()-> std::string {
		std::string str;
		switch (gpuProp.deviceType)
		{
		case  VK_PHYSICAL_DEVICE_TYPE_OTHER:
			str = "Other";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			str = "Integrated GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			str = "Discrete GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			str = "Virtual GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			str = "CPU";
			break;
		default:
			str = "Unknown";
			break;
		}
		return str;
	};
	
	Console_Log("�׷��� ī�� ���� : ");
	Console_Log_Format("API : 0x%x , Driver : 0x%x , VecdorID : 0x%x", gpuProp.apiVersion, gpuProp.driverVersion, gpuProp.vendorID);
	Console_Log_Format("Name \"%s\" , DeviceID : 0x%x , Type \"%s\"", gpuProp.deviceName, gpuProp.deviceID, GetDeviceTypeString().c_str());
	Console_Log_Format("Max Descriptor Sets Bound : %d , Timestamps : %d", gpuProp.limits.maxBoundDescriptorSets, gpuProp.limits.timestampComputeAndGraphics);
}

void VulkanDevice::CreateDevice(bool vaildationLayerOn)
{
	Console_Log("����̽� ���� ����");

	QueueFamilyIndices indices;
	indices.FindQueueFamily(gpu, surface);

	std::set<int> uniqueQueueFamilies = { indices.GetGraphicsFamily(),indices.GetPresentFamily() };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	//TODO : ���߿� ť�� ������ ������� ó�� �� ť�� �켱 ���� ����
	float queuePriority = 1.0f;
	for (size_t i = 0; i < uniqueQueueFamilies.size(); i++)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = i;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	//TODO : ����� GPU�� ����� �����Ѵ�.
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;


	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(Vulkan::deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = Vulkan::deviceExtensions.data();

	if (vaildationLayerOn)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::vaildationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = Vulkan::vaildationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &vkDevice);
	Check_Throw(result != VK_SUCCESS, "����̽� ���� ����!");

	graphicsQueue = new VulkanQueue(this, indices.GetGraphicsFamily(), 0);
	presentQueue = new VulkanQueue(this, indices.GetPresentFamily(), 0);

	Console_Log("����̽� ���� ����");
}

bool VulkanDevice::CheckDeviceFeatures(VkPhysicalDevice device)
{
	QueueFamilyIndices queueFamilyIndices;
	VulkanExtension extension;
	SwapchainSupportDetail supportDetail(device, surface);

	if (queueFamilyIndices.FindQueueFamily(device,surface))
	{
		bool deviceExtensionSupport = extension.CheckDeviceExtensionSupport(device);
		bool swapSupport = supportDetail.CheckSwapchainAdequate();

		//MEMO : ���Ŀ� GPU�� � ����� ����ؾ� �ϴ����� ���� Ȯ�� �� �� ����
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
		bool deviceFeatureSupport = supportedFeatures.samplerAnisotropy;

		return deviceExtensionSupport && deviceFeatureSupport && swapSupport;
	}
	
	return false;
}

void VulkanDevice::CreateSurface(LKEngine::Window::Window* window)
{
	VkResult result = glfwCreateWindowSurface(instance->GetRawInstance(), window->GetWindowHandle(), nullptr, &surface);
	Check_Throw(result != VK_SUCCESS, "Surface�� �������� �ʾҽ��ϴ�!");
}
