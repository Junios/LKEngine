#include "../Header/Window.h"

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Window;

Window::Window(const int width,const int height)
	:
	window(nullptr),
	width(width),
	height(height)
{
	Console_Log("â ���� ����");

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	Check_Throw(window == nullptr, "â ���� ����!");
	Console_Log_If(window, "â ���� ����");
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::WindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

GLFWwindow * LKEngine::Window::Window::GetWindowHandle() const
{
	return window;
}
