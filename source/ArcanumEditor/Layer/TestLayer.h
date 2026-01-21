#pragma once

#include "Layer/ILayer.h"

class TestLayer : public ILayer
{
public:
	TestLayer();
	~TestLayer();

	void OnUpdate(const float& _DeltaTime) override;
};