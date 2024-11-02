#include "Shader.h"

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open the files
		vertexShaderFile.open(vertexFilePath);
		fragmentShaderFile.open(fragmentFilePath);

		// Read the files
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// Close the files
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Read as a string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Failed to read files\n";
		glfwTerminate();
		return;
	}

	// Read as a const char
	vertexShaderData = vertexCode.c_str();
	fragmentShaderData = fragmentCode.c_str();

	int success;
	char infoLog[1024];

	// vertex shader
	this->vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vertexShaderData, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "Failed to create vertex shader" << infoLog << std::endl;
	}

	// fragment shader
	this->fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &fragmentShaderData, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "Failed to create fragment shader" << infoLog << std::endl;
	}

	// Create a program
	this->program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Failed program" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(this->program);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
