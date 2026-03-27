#pragma once

#include <glm/glm.hpp>

struct InputComponent
{
    glm::vec2 mouseInput{};
    glm::vec2 lastMouseInput{};

    glm::vec3 movementInput{};
    bool isPressingShift = false;
};