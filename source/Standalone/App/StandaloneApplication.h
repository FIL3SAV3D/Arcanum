#pragma once

#include "App/Application.h"

class StandaloneApplication : public Application
{
public:
    void Initialize() override;
    void Deinitialize() override;
};