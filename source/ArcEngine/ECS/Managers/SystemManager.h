#pragma once

#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <map>
#include <cassert>

#include "ECS/Data/ECSTypeInfo.h"
#include "ECS/Interfaces/ISystem.h"

#include <GLFW/glfw3.h>
#include "ILayer.h"

class SystemManager
{
public:
	template<typename T, const int priority, typename ...Args>
	std::shared_ptr<T> RegisterSystem(Args ..._args)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>(_args...);
		mSystems.insert({ typeName, system });

		mUpdateOrderHolder[priority] = system;

		return system;
	}

	void RecalculateUpdateOrder()
	{
		mSystemsUpdateOrder.clear();

		for (std::map<int, std::shared_ptr<ISystem>>::iterator it = mUpdateOrderHolder.begin(); it != mUpdateOrderHolder.end(); it++)
		{
			mSystemsUpdateOrder.push_back((*it).second);
		}
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

		// Set the signature for this system
		mSignatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

	void OnCreate()
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnCreate();
		}
	}

	void OnUpdate()
	{
		currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnUpdate(deltaTime);
		}
	}

	void OnDestroy()
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnDestroy();
		}
	}

private:
	float deltaTime = 0.0f;
	float currentFrameTime = 0.0f;
	float lastFrameTime = 0.0f;

	// Map from system type string pointer to a signature
	std::unordered_map<const char*, Signature> mSignatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<ISystem>> mSystems{};

	std::map<int, std::shared_ptr<ISystem>> mUpdateOrderHolder{};
	std::vector<std::shared_ptr<ISystem>> mSystemsUpdateOrder;
};