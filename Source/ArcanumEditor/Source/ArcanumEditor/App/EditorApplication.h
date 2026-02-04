#pragma once

#include "Game/App/Application.h"

class EditorApplication : public Application
{
public:
    EditorApplication() = default;
    ~EditorApplication() = default;

    void Initialize();
    void Deinitialize();
};