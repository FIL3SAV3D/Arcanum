#pragma once

#include <string>

enum DEFAULT_PRIORITIES : uint16_t
{
	EDITOR = 100,
	GAME = 200,
};

class ILayer
{
public:
	ILayer(const std::string& _Name, const uint16_t& _Priority);
	virtual ~ILayer() {};

	virtual void OnAttach() {};
	virtual void OnDetach() {};
	virtual void OnUpdate(const float& _DeltaTime) {};

	inline const std::string& GetName() const { return m_Name; }
	inline const uint16_t& GetPriority() const { return m_Priority; }

private:
	uint16_t m_Priority;
	std::string m_Name;
};