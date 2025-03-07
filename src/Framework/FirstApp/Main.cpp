#include "TempTest.h"

int main()
{
	printf("WELCOME TO ARCANUM USER\n\n");

	try
	{

		arc::cFirstApp app{};
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		system("pause");
		return EXIT_FAILURE;
	}

	system("pause");
	return EXIT_SUCCESS;
}