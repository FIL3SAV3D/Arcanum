#pragma once

#include "Framework/App/IApplication.h"

class Application : public IApplication
{
public:
    Application();
    ~Application();

    // Inherited via IApplication
    void Create() override;
    void Destroy() override;
};