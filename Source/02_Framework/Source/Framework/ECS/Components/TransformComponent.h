#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct TransformComponent
{
    glm::mat4 transform = glm::identity<glm::mat4>();
};