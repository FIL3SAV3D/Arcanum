#include "IMode.h"

IMode::IMode(const std::string& _Name):
	m_LayerStack{std::make_unique<LayerStack>()},
	name{ _Name }
{
}