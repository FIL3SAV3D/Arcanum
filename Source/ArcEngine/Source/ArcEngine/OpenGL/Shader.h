#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "ShaderTemplate.h"

class Shader : public ShaderTemplate
{
public:
	Shader(const char* _name);
	Shader(const char* _vertexPath, const char* _fragmentPath);
	Shader(const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath);
	~Shader();
};

#endif // !SHADER_H
