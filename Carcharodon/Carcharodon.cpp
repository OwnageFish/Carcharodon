// Carcharodon.cpp : Defines the entry point for the application.
//

#include <iostream>

//#include "Carcharodon.h"
//#include "BspProcessor.h"
#include "BSP.h"

int main()
{
	int n = 1;
	if (*(char *)&n == 1)
		std::cout << "Little endian" << std::endl;
	std::cout << "Hello CMake." << std::endl;

	BSP::bsp_data process;

	std::fstream fs;
	fs.open("example.bsp", std::fstream::in | std::fstream::binary);
	BSP::readFromFile(fs, process);
	BSP::coutData(process);

	return 0;
}
