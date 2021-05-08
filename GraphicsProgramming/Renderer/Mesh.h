#pragma once
#include "../Textures/Texture2D.h"
#include "../Shaders/Shader.h"
#include "../Core/VertexArray.h"


class Mesh
{
public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	Texture2D* textures;

	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, Texture2D* textures) noexcept;
	
	Mesh(Mesh&& other) noexcept;

	void Draw(Shader& shader);
private:
	VertexArray<float, unsigned int> VAO;
};
