#include "ShaderTemplate.h"

// Glad
#include <glad/glad.h>

// GLM Type Ptr
#include <glm/gtc/type_ptr.hpp>

// STD
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

void ShaderTemplate::Use()
{
	glUseProgram(ID);
}

void ShaderTemplate::DestroyShader()
{
	glDeleteProgram(ID);
}

void ShaderTemplate::setBool(const char* _name, const bool& _value) const
{
	glUniform1i(glGetUniformLocation(ID, _name), (int)_value);
}

void ShaderTemplate::setInt(const char* _name, const int& _value) const
{
	glUniform1i(glGetUniformLocation(ID, _name), _value);
}

void ShaderTemplate::setFloat(const char* _name, const float& _value) const
{
	glUniform1f(glGetUniformLocation(ID, _name), _value);
}

void ShaderTemplate::setVec3(const char* _name, const glm::vec3& _value) const
{
	glUniform3f(glGetUniformLocation(ID, _name), _value.x, _value.y, _value.z);
}

void ShaderTemplate::setVec4(const char* _name, const glm::vec4& _value) const
{
	glUniform4f(glGetUniformLocation(ID, _name), _value.x, _value.y, _value.z, _value.w);
}

void ShaderTemplate::setMat3(const char* _name, const glm::mat3& _value) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, _name), 1, GL_FALSE, glm::value_ptr(_value));
}

void ShaderTemplate::setMat4(const char* _name, const glm::mat4& _value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, _name), 1, GL_FALSE, glm::value_ptr(_value));
}

void ShaderTemplate::CreateShader(const char* _vertexShaderPath, const char* _fragmentShaderPath, const char* _geometryShaderPath)
{
	std::string directory;
	std::string currentPath = std::filesystem::current_path().string();
	const size_t last_slash_idx = currentPath.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		directory = currentPath.substr(0, last_slash_idx);
	}

	std::string shaderSearchPath;
	shaderSearchPath.append(directory);
	shaderSearchPath.append("\\Arcanum");
	shaderSearchPath.append("\\source\\ArcEngine\\OpenGL\\Shaders\\");

	ID = glCreateProgram();

	int vertID = CreateVertexShader((shaderSearchPath + std::string(_vertexShaderPath)).c_str());
	int fragID = CreateFragmentShader((shaderSearchPath + std::string(_fragmentShaderPath)).c_str());

	int geomID;
	if (_geometryShaderPath)
	{
		geomID = CreateGeometryShader((shaderSearchPath + std::string(_geometryShaderPath)).c_str());
	}

	glLinkProgram(ID);

	CheckCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertID);
	glDeleteShader(fragID);

	if (_geometryShaderPath)
	{
		glDeleteShader(geomID);
	}

}

int ShaderTemplate::CreateVertexShader(const char* _vertexShaderPath)
{
	std::string data{ ReadFile(_vertexShaderPath) };
	const char* code = data.c_str();

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &code, nullptr);
	glCompileShader(vertex);

	CheckCompileErrors(vertex, "VERTEX");

	glAttachShader(ID, vertex);

	return vertex;
}

int ShaderTemplate::CreateFragmentShader(const char* _fragmentShaderPath)
{
	std::string data{ ReadFile(_fragmentShaderPath) };
	const char* code = data.c_str();

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &code, nullptr);
	glCompileShader(fragment);

	CheckCompileErrors(fragment, "FRAGMENT");

	glAttachShader(ID, fragment);

	return fragment;
}

int ShaderTemplate::CreateGeometryShader(const char* _geometryShaderPath)
{
	std::string data{ ReadFile(_geometryShaderPath) };
	const char* code = data.c_str();

	unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(geometry, 1, &code, nullptr);
	glCompileShader(geometry);

	CheckCompileErrors(geometry, "VERTEX");

	glAttachShader(ID, geometry);

	return geometry;
}

std::string ShaderTemplate::ReadFile(const char* _filePath)
{
	std::string code;
	std::ifstream ShaderFile;
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		ShaderFile.open(_filePath);
		std::stringstream ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();
		code = ShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << _filePath << std::endl;
	}

	return code;
}

void ShaderTemplate::CheckCompileErrors(unsigned int shader, const char* type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}