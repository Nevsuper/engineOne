#include "pch.h"
#include "Application.h"
#include "Engine.h"
#include<iostream>

int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	Application* pApp = new Application("My 3D Application");
	Engine engine(hInstance, pApp);
	if (!engine.Run())
	{
		std::cout << "Failed torun game." << std::endl;
		return -1;
	}
	return 0;
}