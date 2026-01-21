#include <iostream>

#include "StandaloneGame.h"

#include "StandaloneMode.h"

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    try
    {
        StandaloneGame* game = new StandaloneGame();
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