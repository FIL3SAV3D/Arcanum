#pragma once

#include "ILayer.h"
#include <vector>

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(ILayer* layer);
	void PopLayer(ILayer* layer);

	std::vector<ILayer*>::iterator begin() { return m_Layers.begin(); }

private:
	std::vector<ILayer*> m_Layers;
};