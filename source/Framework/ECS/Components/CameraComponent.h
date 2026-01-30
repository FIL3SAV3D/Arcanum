#pragma once

#include "Jolt/Jolt.h"
#include "Jolt/Math/Mat44.h"

class CameraComponent
{
public:
    JPH::Vec3 forward;
    JPH::Vec3 position;
};