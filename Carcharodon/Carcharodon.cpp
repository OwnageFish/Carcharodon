// Carcharodon.cpp : Defines the entry point for the application.
//

#include "Carcharodon.h"


int main()
{
	int n = 1;
	if (*(char *)&n == 1)
		std::cout << "Little endian" << std::endl;
	std::cout << "Hello CMake." << std::endl;
	return 0;
}
