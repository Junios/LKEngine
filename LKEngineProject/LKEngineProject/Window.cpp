#include "Window.h"

#include "Macro.h"

using namespace LKEngine;

LKEngine::Window::Window::Window(const int width,const int height)
	:
	window(nullptr),
	width(width),
	height(height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	Check_Throw(window == nullptr, "â ���� ����!");
	Console_Log_If(window, "â ���� ����");
}

LKEngine::Window::Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool LKEngine::Window::Window::WindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void LKEngine::Window::Window::PollEvents()
{
	glfwPollEvents();
}
