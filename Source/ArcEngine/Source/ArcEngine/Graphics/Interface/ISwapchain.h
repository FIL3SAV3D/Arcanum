#pragma once

#include "glm/glm.hpp"

class ISwapchain
{
public:
    virtual void Create(const glm::uvec2& _Size) = 0;
    virtual void Destroy() = 0;

};