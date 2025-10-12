#include "Shader.h"

#include <string>

Shader::Shader(const char* _name)
{
	std::string vertName{_name + std::string(".vert")};
	std::string fragName{ _name + std::string(".frag") };
	CreateShader(vertName.c_str() , fragName.c_str());
}

Shader::Shader(const char* _vertexPath, const char* _fragmentPath)
{
	CreateShader(_vertexPath, _fragmentPath);
}

Shader::Shader(const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath)
{
	CreateShader(_vertexPath, _fragmentPath, _geometryPath);
}

Shader::~Shader()
{
	DestroyShader();
}