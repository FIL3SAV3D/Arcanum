#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <filesystem>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader
{
public:
	unsigned int ID{ 0 };

	Shader(const char* _vertexPath, const char* _fragmentPath);
	~Shader();

	void use();

	void setBool (const std::string& _name, const bool&				 _value) const;
	void setInt	 (const std::string& _name, const int&				 _value) const;
	void setFloat(const std::string& _name, const float&			 _value) const;
	void setVec3f(const std::string& _name, const glm::tvec3<float>& _value) const;
	void setVec4f(const std::string& _name, const glm::tvec4<float>& _value) const;
};

#endif // !SHADER_H
