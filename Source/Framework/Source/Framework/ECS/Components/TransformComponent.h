#pragma once

#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"
#include "Jolt/Math/Quat.h"
#include "Jolt/Math/Mat44.h"

struct TransformComponent
{
    JPH::Mat44 transform;
};