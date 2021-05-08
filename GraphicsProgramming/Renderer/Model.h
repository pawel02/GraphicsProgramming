#pragma once
#include "../Shaders/Shader.h"
#include "../Textures/Texture2D.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const char* filepath) noexcept
	{
		loadModel(filepath);
	};

	void Draw(Shader& shader);
private:
	void loadModel(const char* filepath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	
private:
	std::vector<Mesh> meshes;
	std::string directory;

	std::vector<Texture2D> loadedTextures;
};
