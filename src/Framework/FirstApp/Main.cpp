#include "temp/FirstApp.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	printf("WELCOME TO ARCANUM USER\n\n");
	arc::cFirstApp app{};

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}