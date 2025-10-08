#include "ShaderManager.h"

#include <filesystem>

#include <iostream>
#include <fstream> 

#include <glad/glad.h>


ShaderManager::ShaderManager()
{
	ShaderSearchPath.append(std::filesystem::current_path().string());
	ShaderSearchPath.append("\\source\\ArcEngine\\OpenGL\\Shaders\\");
}

ShaderManager::~ShaderManager()
{
	DestroyAllShaders();
}


void ShaderManager::DestroyAllShaders()
{
	size_t shaderCount	= shaders.size();

	for (size_t i = 0; i < shaderCount; i++)
	{
		unsigned int itr = shaders.at(i);
		glDeleteShader(itr);
	}
}