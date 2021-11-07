// Carcharodon.cpp : Defines the entry point for the application.
//

#include <iostream>

//#include "Carcharodon.h"
#include "BspProcessor.h"

int main()
{
	int n = 1;
	if (*(char *)&n == 1)
		std::cout << "Little endian" << std::endl;
	std::cout << "Hello CMake." << std::endl;

	BspProcessor process;
	process.read_from_file("./test.txt");
	//process.read_from_file("example.bsp");

	return 0;
}
