#include "ImGUILayer.h"

ImGUILayer::ImGUILayer(std::shared_ptr<Window> _Window):
	ILayer{ "ImGUI", 50 }
{
	window = _Window;
}

ImGUILayer::~ImGUILayer()
{
}

void ImGUILayer::OnAttach()
{
	imGUIHandler = std::make_unique<ImGUIHandler>();
	imGUIHandler->Initialize(window);
}

void ImGUILayer::OnDetach()
{
	imGUIHandler->DeInitialize();
	imGUIHandler.reset();
}

void ImGUILayer::OnUpdate(const float& _DeltaTime)
{
	imGUIHandler->Update();
}