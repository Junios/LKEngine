#include "Application/Header/Application.h"
#include <crtdbg.h>

int main()
{
	using namespace LKEngine::Application;

	Application app(800,600);

	app.Loop();

	//�޸� ���� üũ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 0;
}