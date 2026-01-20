#pragma once

#include "ILayer.h"
#include <vector>
#include <memory>

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(std::shared_ptr<ILayer> layer);
	void PopLayer(std::shared_ptr<ILayer> layer);

	std::vector<std::shared_ptr<ILayer>> m_Layers;
private:
	unsigned int m_LayerInsertIndex;

};