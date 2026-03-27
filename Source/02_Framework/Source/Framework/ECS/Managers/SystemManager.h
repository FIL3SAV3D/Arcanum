#pragma once

#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <map>
#include <cassert>

#include "Framework/ECS/Data/ECSTypeInfo.h"
#include "Framework/ECS/Interfaces/ISystem.h"
#include "Framework/States/States.h"

class Coordinator;

class SystemManager
{
public:
	template<typename T, const int priority, typename ...Args>
	std::shared_ptr<ISystem> RegisterSystem(ComponentManager& _ComponentManager, Args ..._args)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally
		std::shared_ptr<ISystem> system = std::static_pointer_cast<ISystem>(std::make_shared<T>(_args...));
		mSystems.insert({ typeName, system });

		mUpdateOrderHolder[priority] = system;

		SignatureParameters params{
		.componentManager = _ComponentManager,
		};
		system->GetSignature(params);
		SetSignature<T>(params.signature);

		RecalculateUpdateOrder();

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

	void OnCreate(StartState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnCreate(_State);
		}
	}

	void OnStart(StartState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnStart(_State);
		}
	}

	void OnInput(InputState& _InputState)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnInput(_InputState);
		}
	}

	void OnUpdate(GameState& _State, const float& _DeltaTime)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnUpdate(_State, _DeltaTime);
		}
	}

	void OnLateUpdate(GameState& _State, const float& _DeltaTime)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnLateUpdate(_State, _DeltaTime);
		}
	}

	void OnRenderStart(RenderState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnRenderStart(_State);
		}
	}

	void OnRender(RenderState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnRender(_State);
		}
	}

	void OnRenderEnd(RenderState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnRenderEnd(_State);
		}
	}

	void OnRenderUI(RenderState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnRenderUI(_State);
		}
	}

	void OnApplicationPause()
	{
	}

	void OnCheckForDisabled()
	{
	}

	void OnDestroy(EndState& _State)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnDestroy(_State);
		}
	}

	void OnResize(const glm::uvec2& _Size)
	{
		for (const std::shared_ptr<ISystem> system : mSystemsUpdateOrder)
		{
			system->OnWindowResize(_Size);
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