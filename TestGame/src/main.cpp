#include <engineOne/Core/Engine.h>
#include <engineOne/Core/logger/Logger.h>
#include "Application.h"
#include<iostream>

int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	Engine engine(hInstance, std::make_unique<Application>("My 3D Application"));
	if (!engine.Run())
	{
		LOG_FATAL("Failed torun game.");
		return -1;
	}
	return 0;
}