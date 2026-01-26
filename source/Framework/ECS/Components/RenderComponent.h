#pragma once

#include "OpenGL/Model.h"
#include "OpenGL/Shader.h"

struct RenderComponent
{
	bool instanced = false;
	std::shared_ptr<Model> model;
	std::shared_ptr<Shader> shader;
};