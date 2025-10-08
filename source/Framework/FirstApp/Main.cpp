#include "TempTest.h"

#include "OpenGL/OGLSystem.h"

int main()
{
	printf("WELCOME TO ARCANUM USER\n\n");

	try
	{
		OGLSystem OGLApp{};
		OGLApp.Run();

		//arc::cFirstApp app{};
		//app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		system("pause");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}