#include "EditorApplication.h"

void EditorApplication::Initialize()
{
    Application::Create();
}

void EditorApplication::Deinitialize()
{
    Application::Destroy();
}
