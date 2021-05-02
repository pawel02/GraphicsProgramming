#pragma once
#include <glad/glad.h>
#include <vector>
#include <unordered_map>

/*
Shader wrapper used to load in shaders
*/
class Shader
{

public:
	Shader() noexcept;
	Shader(const char* vertexFile, const char* fragmentFile) noexcept;

	~Shader() noexcept;

	void load_shader(unsigned int shader_type, const char* shader_file);
	void delete_shaders();
	void create_program();

	//binds the shader program
	void bind() const noexcept;

//all of the possible uniforms
public:
	void set_uniform_4f(const char* uniform_name, float x, float y, float z, float w) noexcept;
	void set_uniform_1i(const char* uniform_name, int i);
	void set_uniform_1f(const char* uniform_name, float i);

private:
	int get_uniform_location(const char* uniform_name) noexcept;

private:
	std::vector<unsigned int> _shaders;
	unsigned int _program = 0;

	//for errors
	int success = 0;
	char infoLog[512] = "";

	std::unordered_map<const char*, int> uniform_locations;
};