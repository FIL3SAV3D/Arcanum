#pragma once

#include <glm/glm.hpp>

struct CameraData
{
    glm::vec4 position;
    glm::mat4 view;
    glm::mat4 projection;
};