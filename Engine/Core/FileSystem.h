#pragma once
#include <string>

namespace gme {
	void SetFilePath(const std::string& pathname);
	bool ReadFileToString(const std::string& filenmae, std::string& filestring);
	std::string GetFilePath();
}