#include <iostream>

#include "ArcEngine.h"
#include "ECS/Interfaces/ISystem.h"

int main()
{
    std::printf("WELCOME TO ARCANUM USER\n\n");

    try
    {
        ArcEngine engine;
        engine.Run();
    }
    catch (const std::exception& e)
    {
        std::printf(e.what());
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}