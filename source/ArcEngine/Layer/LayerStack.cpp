#include "LayerStack.h"

#include <algorithm>
#include <format>

LayerStack::LayerStack():
	m_LayerInsertIndex{0}
{
}

LayerStack::~LayerStack()
{
	size_t length = m_LayerPriorities.size();
	for (size_t itr = 0; itr < length; itr++)
	{
		auto layer = m_LayerPriorities.popback();
		layer->OnDetach();
	}
}

void LayerStack::PopLayer(std::shared_ptr<ILayer> layer)
{
	layer->OnDetach();
	m_LayerPriorities.remove(layer);
}

void LayerStack::Update(const float& _DeltaTime)
{
	//int i = 0;
	for (auto layer : m_LayerPriorities)
	{
		//std::printf(std::format("{}: Layer: {} Priority: {}\n", i, layer->GetName(), layer->GetPriority()).c_str());

		layer->OnUpdate(_DeltaTime);
		//i++;
	}
}