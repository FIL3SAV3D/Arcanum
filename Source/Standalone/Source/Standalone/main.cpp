#include <iostream>

#include "App/StandaloneApplication.h"

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    try
    {
        StandaloneApplication* game = new StandaloneApplication();
        game->Create();

        while (!game->IsQuitting())
        {
            game->Run();
        }

        game->Destroy();
    }
    catch (const std::exception& e)
    {
        std::printf(e.what());
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}