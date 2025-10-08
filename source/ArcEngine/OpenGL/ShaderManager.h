#pragma once

#include <vector>
#include <string>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	unsigned int getShaderByName(const std::string& _name);

	void DestroyAllShaders();

private:
	std::string ShaderSearchPath;

	std::vector<unsigned int> shaders{};
};