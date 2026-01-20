#include <iostream>

#include "App/Application.h"

#include "StandaloneMode.h"

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    try
    {
        Application* application = new Application();
        application->Create();

        while (!application->IsQuitting())
        {
            application->Run();
        }

        application->Destroy();
    }
    catch (const std::exception& e)
    {
        std::printf(e.what());
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}