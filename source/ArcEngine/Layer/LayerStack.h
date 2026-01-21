#pragma once

#include "ILayer.h"
#include <vector>
#include <memory>
#include <cassert>

#include "Misc/AdvancedPriorityQueue.h"

class CompareLayerGreater
{
public:
	bool operator()(std::shared_ptr<ILayer> _Left, std::shared_ptr<ILayer> _Right)
	{
		return (_Left->GetPriority()) < (_Right->GetPriority());
	}
};

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	template<typename T, typename ...Args>
	std::shared_ptr<ILayer> PushLayer(Args... _Args)
	{
		std::shared_ptr<ILayer> layer = std::static_pointer_cast<ILayer>(std::make_shared<T>(_Args...));

		if (!m_LayerPriorities.empty())
		{
			assert(!m_LayerPriorities.contains(layer) && "Layer Already Exists");
		}

		m_LayerPriorities.push(layer);
		layer->OnAttach();
		return layer;
	}
	void PopLayer(std::shared_ptr<ILayer> _Layer);

	void Update(const float& _DeltaTime);

private:
	AdvancedPriorityQueue<std::shared_ptr<ILayer>, std::vector<std::shared_ptr<ILayer>>, CompareLayerGreater> m_LayerPriorities;

	unsigned int m_LayerInsertIndex;

};

