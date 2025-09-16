#include "pch.h"
#include "Application.h"
#include<iostream>

int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	Application app(hInstance, "My 3D Application");
	if (!app.Init())
	{
		std::cout << "Failed to initialize application." << std::endl;
		return -1;
	}
	app.Run();
	return 0;
}