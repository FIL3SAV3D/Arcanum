#pragma once

#include "App/IApplication.h"

class Application : public IApplication
{
public:
    Application();
    ~Application();

    // Inherited via IApplication
    void Initialize();
    void Deinitialize();
};