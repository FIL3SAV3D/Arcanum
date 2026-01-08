#pragma once

#include <memory>
#include <string>

class Window;
class InputHandler;
class Coordinator;

class ArcEngine
{
public:
     ArcEngine();
    ~ArcEngine();

    void Run();

private:
    std::string windowName = "Arcanum";

    int screenWidth = 800;
    int screenHeight = 600;

    std::shared_ptr<Window> window;

    std::shared_ptr<Coordinator> coordinator;

    std::shared_ptr<InputHandler> inputHandler;

};