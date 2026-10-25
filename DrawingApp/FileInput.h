#pragma once
#include <filesystem>

namespace FileInput
{
	void loadFilePathString(std::string dirPathString, std::vector<std::string>& targetVector);
	std::string getNameFromFilePath(std::string str, std::string dirPath);
}

