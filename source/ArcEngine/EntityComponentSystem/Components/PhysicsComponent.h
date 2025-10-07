#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "Temp/JoltHelper.h"

using namespace JPH;


class PhysicsComponent
{
public:
	void CreateCollision(PhysicsSystem& interface, JPH::Array<JPH::Vec3> verts)
	{
		CollisionID = BodyCreator::CreateBody(interface, verts);
	}

	BodyID CollisionID;
};