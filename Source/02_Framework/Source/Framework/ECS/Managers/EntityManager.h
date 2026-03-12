#pragma once

#include <queue>
#include <array>
#include <assert.h>

#include "Framework/ECS/Data/ECSTypeInfo.h"

class EntityManager
{
public:
	EntityManager()  = default;
	~EntityManager() = default;

protected:
	void Create()
	{
		m_AliveEntities.resize(MAX_ENTITIES);

		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}

	void Destroy()
	{
		size_t size = m_AliveEntities.size();
		for (size_t i = 0; i < size; i++)
		{
			Entity entity = m_AliveEntities.at(i);
			DestroyEntity(entity);
		}
	}

	friend class Coordinator;

public:
	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existance");

		Entity id = m_AvailableEntities.front();
		m_AvailableEntities.pop();

		
		++mLivingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range");

		mSignatures[entity].reset();

		m_AvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return mSignatures[entity];
	}

private:
	std::queue<Entity> m_AvailableEntities{};

	std::vector<Entity> m_AliveEntities{};

	std::array<Signature, MAX_ENTITIES> mSignatures{};

	uint32_t mLivingEntityCount{};
};