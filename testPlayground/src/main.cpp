#include <engineOne/Core/Engine.h>
#include <engineOne/Core/logger/Logger.h>
#include "Application.h"
#include<iostream>

int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	std::unique_ptr<Application> pApp = std::make_unique<Application>("My 3D Application");
	Engine engine(hInstance, std::move(pApp));
	if (!engine.Run())
	{
		LOG_FATAL("Failed torun game.");
		return -1;
	}
	return 0;
}