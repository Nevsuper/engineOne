#include "pch.h"
#include "utils.h"
#include<fstream>
#include<engineOne/Core/logger/Logger.h>

bool LoadStringFromFile(const std::string& fileName, std::string& fileAsString)
{
	try 
	{

		std::ifstream file(fileName, std::ios::in);
		if (!file)
		{
			throw std::runtime_error("Failed to open file :" + fileName);
		}

		std::ostringstream oss;
		oss << file.rdbuf();
		fileAsString = oss.str();
		return true;
	}
	catch (const std::exception& e)
	{
		LOG_WARN("file loading exception : {} ", e.what());
		return false;
	}

}
