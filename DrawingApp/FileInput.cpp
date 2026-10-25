#include "FileInput.h"

#include <vector>
#include <string>

namespace FileInput 
{
	// loads in file names from a directory 
	void loadFilePathString(std::string dirPathString, std::vector<std::string>& targetVector)
	{
		// adding '/' at the end of directory path to ensure correct logic
		if (dirPathString[dirPathString.length() - 1] != '/')
			dirPathString += '/';
		std::filesystem::path path(dirPathString);
		// store all files from given directory
		for (auto const& dirPath : std::filesystem::directory_iterator(path))
			targetVector.push_back(dirPath.path().generic_string());
	}

	std::string getNameFromFilePath(std::string str, std::string dirPath)
	{
		// keep only file name
		std::string noPrefix = str.substr(dirPath.length());

		std::size_t fileExtensionBegining = noPrefix.find('.');
		if (fileExtensionBegining == std::string::npos)
			throw std::runtime_error("Make sure path " + dirPath + " does not contain any directories!");

		// return filename without extension
		return noPrefix.substr(0, fileExtensionBegining);
	}
}