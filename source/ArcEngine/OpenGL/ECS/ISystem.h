#pragma once

class ISystem
{
	//Runs from top to bottom
	virtual void OnCreate() = 0;

	virtual void OnStart() = 0;
	
	//Not Implemented yet
	virtual void OnFixedUpdate(const float& _deltaTime) = 0;

	virtual void OnUpdate(const float& _deltaTime) = 0;

	virtual void OnRender() = 0;

	virtual void OnRenderPost() = 0;

	virtual void OnLateUpdate(const float& _deltaTime) = 0;

	virtual void OnStop() = 0;
	
	virtual void OnDestroy() = 0;
};