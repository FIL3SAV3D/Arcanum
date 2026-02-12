#pragma once

#include "ArcEngine/Asset/Types/Model.h"
#include "ArcEngine/Graphics/OpenGL-4.6/Shader.h"

#include <memory>

struct RenderComponent
{
	bool instanced = false;
	std::shared_ptr<Shader> shader;
	Model model;
};