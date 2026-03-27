#pragma once

#include <glm/glm.hpp>

struct CameraComponent
{
    glm::mat4 view       = glm::identity<glm::mat4>();
    glm::mat4 projection = glm::identity<glm::mat4>();
};