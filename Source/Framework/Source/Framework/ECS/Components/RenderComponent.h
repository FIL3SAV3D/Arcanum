#pragma once

#include "Asset/Types/Model.h"
#include "OpenGL/Shader.h"

struct RenderComponent
{
	bool instanced = false;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Model> model;
};