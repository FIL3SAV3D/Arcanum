#include "Shader.h"

Shader::Shader(const char* _vertexPath, const char* _fragmentPath)
{
	std::string shaderSearchPath;
	shaderSearchPath.append(std::filesystem::current_path().string());
	shaderSearchPath.append("\\source\\ArcEngine\\OpenGL\\Shaders\\");

	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(shaderSearchPath + _vertexPath);
		fShaderFile.open(shaderSearchPath + _fragmentPath);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex		= 0;
	unsigned int fragment	= 0;

	int success = 0;

	char infoLog[512];

	vertex		= glCreateShader(GL_VERTEX_SHADER);
	fragment	= glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex,   1, &vShaderCode, nullptr);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);

	glCompileShader(vertex);
	glCompileShader(fragment);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	ID = glCreateProgram();

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& _name, const bool& _value) const
{
	glUniform1i(glGetUniformLocation(ID, _name.c_str()), (int)_value);
}

void Shader::setInt(const std::string& _name, const int& _value) const
{
	glUniform1i(glGetUniformLocation(ID, _name.c_str()), _value);
}

void Shader::setFloat(const std::string& _name, const float& _value) const
{
	glUniform1f(glGetUniformLocation(ID, _name.c_str()), _value);
}

void Shader::setVec3f(const std::string& _name, const glm::tvec3<float>& _value) const
{
	glUniform3f(glGetUniformLocation(ID, _name.c_str()), _value.x, _value.y, _value.z);
}

void Shader::setVec4f(const std::string& _name, const glm::tvec4<float>& _value) const
{
	glUniform4f(glGetUniformLocation(ID, _name.c_str()), _value.x, _value.y, _value.z, _value.w);
}
