#include <iostream>

#include "ArcanumEditor/App/EditorApplication.h"

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    try
    {
        EditorApplication* application = new EditorApplication();
        application->OnCreate();

        while (!application->bQuit)
        {
            application->Run();
        }

        application->OnDestroy();
    }
    catch (const std::exception& e)
    {
        std::printf(e.what());
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}