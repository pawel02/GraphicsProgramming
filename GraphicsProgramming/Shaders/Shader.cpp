#include "Shader.h"
#include <fstream>
#include <string>
#include <assert.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexFile, const char* fragmentFile) noexcept
{
	load_shader(GL_VERTEX_SHADER, vertexFile);
	load_shader(GL_FRAGMENT_SHADER, fragmentFile);

	create_program();
	delete_shaders();
}

Shader::Shader() noexcept
{
	//usually you will only need the vertex and fragment shaders
	_shaders.reserve(2);
}

Shader::~Shader() 
{
	glDeleteProgram(_program);
}

void Shader::load_shader(unsigned int shader_type, const char* shader_file)
{
	//get the source code for the shader
	std::ifstream file(shader_file);
	if (file.is_open())
	{
		std::string shader_source((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		//create the shader
		unsigned int shader = glCreateShader(shader_type);
		_shaders.emplace_back(shader);
		const char* source = shader_source.c_str();
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);


#ifdef _DEBUG
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		//do some error checking
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			assert(infoLog);
		}
#endif
	}
	else
	{
		std::cout << "Could not load the shader source code\n";
	}

}

void Shader::delete_shaders() 
{
	for (auto&& shader : _shaders)
	{
		glDeleteShader(shader);
	}
}

void Shader::create_program()
{
	_program = glCreateProgram();
	for (auto&& shader : _shaders)
	{
		glAttachShader(_program, shader);
	}

	glLinkProgram(_program);

	//catch any errors in debug move
#ifdef _DEBUG
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		assert(infoLog);
	}
#endif

	bind();
}

void Shader::bind() const noexcept
{
	glUseProgram(_program);
}

//define all of the uniforms
int Shader::get_uniform_location(const char* uniform_name) noexcept
{
	if (uniform_locations.find(uniform_name) != uniform_locations.end())
	{
		return uniform_locations.at(uniform_name);
	}

	int uniform_location = glGetUniformLocation(_program, uniform_name);
	uniform_locations.insert(std::pair<const char*, int>{ uniform_name, uniform_location });
#ifdef _DEBUG
	if (uniform_location < 0)
	{
		assert("Could not find the unifrom");
	}
#endif

	//making sure that the program is bound
	bind();
	return uniform_location;
}

void Shader::set_uniform_mat4f(const char* uniform_name, const glm::mat4& matrix)
{
	int uniform_location = get_uniform_location(uniform_name);
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_uniform_4f(const char* uniform_name, float x, float y, float z, float w) noexcept
{
	int uniform_location = get_uniform_location(uniform_name);
	glUniform4f(uniform_location, x, y, z, w);
}

void Shader::set_uniform_1i(const char* uniform_name, int i)
{
	int uniform_location = get_uniform_location(uniform_name);
	glUniform1i(uniform_location, i);
}

void Shader::set_uniform_1f(const char* uniform_name, float i)
{
	int uniform_location = get_uniform_location(uniform_name);
	glUniform1f(uniform_location, i);
}