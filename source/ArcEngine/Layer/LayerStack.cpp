#include "LayerStack.h"

#include <algorithm>

LayerStack::LayerStack():
	m_LayerInsertIndex{0}
{
}

LayerStack::~LayerStack()
{
	for (auto layer = m_Layers.rbegin(); layer != m_Layers.rend(); layer++)
	{
		layer->get()->OnDetach();
		layer->reset();
	}
}

void LayerStack::PushLayer(std::shared_ptr<ILayer> layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void LayerStack::PopLayer(std::shared_ptr<ILayer> layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
	if (it != m_Layers.begin() + m_LayerInsertIndex)
	{
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}
