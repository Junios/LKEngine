#include "Application/Header/Application.h"

#include <cstdlib>
#include <crtdbg.h>

int main()
{
	using namespace LKEngine::Application;

	Application* app = new Application(800,600);

	app->Loop();

	delete app;

	//�޸� ���� üũ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 0;
}