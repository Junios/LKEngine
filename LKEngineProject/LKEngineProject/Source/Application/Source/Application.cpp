#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Renderer/Header/VulkanDevice.h"

#include "../../Application/Header/PipelineManager.h"
#include "../../Application/Header/SceneManager.h"
#include "../../Application/Header/EntityPool.h"
#include "../../Application/Header/Time.h"

using namespace LKEngine::Application;

#include "../../../TestScene.h"

Application::Application(const int windowWidth, const int windowHeight)
{
	Console_Log("�ʱ�ȭ ����");
	window = new Window::WindowsWindow(windowWidth,windowHeight);
	device = Vulkan::VulkanDevice::GetInstance();

	device->SetDebugMode(true);
	device->SetWindow(window);

	window->Init(device);
	device->Init();

	LKEngine::Time::Start();

	SceneManager::GetInstance()->Start(new TestScene());

	Console_Log("�ʱ�ȭ ����");

}

Application::~Application()
{
	device->WaitIdle();

	SceneManager::Release();
	EntityPool::Release();
	PipelineManager::Release();
	

	Vulkan::VulkanDevice::Release();
	window->Shutdown();

	SAFE_DELETE(window);
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose())
	{
		LKEngine::Time::Update();

		SceneManager::GetInstance()->Update();
		EntityPool::GetInstance()->Update();

		if (EntityPool::GetInstance()->NeedRender()) device->Render();

		device->Draw();

		window->PollEvents();
	}
}
