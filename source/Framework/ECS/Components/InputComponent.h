#pragma once

#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"

struct InputComponent
{
    JPH::Vec3 direction;
    double xPos;
    double yPos;

    bool mouseOne;
    bool mouseTwo;
};