#pragma once

#include <set>

#include "ECS/Data/ECSTypeInfo.h"
#include <OpenGL/IRenderer.h>

class ISystem
{
public:
    virtual void OnCreate() {};

	virtual void OnInput() {};
	virtual void OnUpdate(const float& _DeltaTime) {};
	virtual void OnLateUpdate(const float& _DeltaTime) {};
	virtual void OnRender(std::shared_ptr<IRenderer> _Renderer) {};
	virtual void OnRenderUI(std::shared_ptr<IRenderer> _Renderer) {};
	virtual void OnApplicationPause() {};
	virtual void OnCheckForDisabled() {};

    virtual void OnDestroy() {};

public:
    std::set<Entity> mEntities{};
};