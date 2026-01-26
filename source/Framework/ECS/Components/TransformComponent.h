#pragma once

#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"
#include "Jolt/Math/Quat.h"

struct TransformComponent
{
    JPH::Vec3 position;
    JPH::Quat rotation;
    JPH::Vec3 scale;
};