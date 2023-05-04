#include "resourceLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const char* filename)
{
	std::string string;
	std::stringstream stringBuffer[1];
	std::ifstream File(filename);

	bool begin = false;

	while (std::getline(File, string)) {
		stringBuffer[0] << string << "\n";
	}

	std::cout << stringBuffer[0].str() << std::endl;

	return stringBuffer[0].str();
}