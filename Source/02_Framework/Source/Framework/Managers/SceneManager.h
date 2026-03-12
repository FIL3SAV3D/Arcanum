#pragma once

#include <string>
#include <Framework/Scenes/IScene.h>
#include <unordered_map>

class SceneManager
{
public:
    std::unordered_map<std::string, IScene> m_SceneMap;
};