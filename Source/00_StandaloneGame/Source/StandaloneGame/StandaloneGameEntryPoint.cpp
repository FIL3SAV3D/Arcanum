#include <iostream>

#include <Game/Game.h>

int main(int /*NumArgs*/, char** /*ppArgs*/)
{
    system("cls");

    try
    {
        Game* game = new Game();

        game->Create();

        // Game Loop
        game->OnEnter();

        while (!game->IsQuitting())
        {
            game->Run();
        }

        game->OnExit();
        // Game Loop

        game->Destroy();

        delete game;
    }
    catch (const std::exception& e)
    {
        std::printf(e.what());
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}