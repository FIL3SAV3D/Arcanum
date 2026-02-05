#pragma once

#include "ArcEngine/Asset/Types/Model.h"
#include "ArcEngine/OpenGL/Shader.h"

#include <memory>

struct RenderComponent
{
	bool instanced = false;
	std::shared_ptr<Shader> shader;
	Model model;
};