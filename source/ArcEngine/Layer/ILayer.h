#pragma once

#include <string>

class ILayer
{
public:
	ILayer(const std::string& name);
	virtual ~ILayer();

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate();

	inline const std::string& GetName() const { return m_debugName; }

private:
	std::string m_debugName;
};