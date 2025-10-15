#pragma once

#ifndef I_SHADER_H
#define I_SHADER_H

// GLM Types
#include <glm/vec3.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>

class ShaderTemplate
{
public:
	unsigned int ID{ 0 };

public:
	void Use();

	void CreateShader(const char* _vertexShaderPath, const char* _fragmentShaderPath, const char* _geometryShaderPath = nullptr);
	void DestroyShader();

	void setBool	(const char* _name, const bool&		 _value) const;
	void setInt		(const char* _name, const int&		 _value) const;
	void setFloat	(const char* _name, const float&	 _value) const;

	void setVec3	(const char* _name, const glm::vec3& _value) const;
	void setVec4	(const char* _name, const glm::vec4& _value) const;

	void setMat3	(const char* _name, const glm::mat3& _value) const;
	void setMat4	(const char* _name, const glm::mat4& _value) const;

protected:
	int CreateVertexShader	(const char* _vertexShaderPath);
	int CreateFragmentShader(const char* _fragmentShaderPath);
	int CreateGeometryShader(const char* _geometryShaderPath);

private:
	std::string ReadFile(const char* _filePath);

	void CheckCompileErrors(unsigned int shader, const char* type);
};

#endif // !I_SHADER_H
