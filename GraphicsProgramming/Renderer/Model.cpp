#include "Model.h"
#include <iostream>

void Model::Draw(Shader& shader)
{
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::loadModel(const char* filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
		return;
	}
	directory = std::string{ filepath }.substr(0, std::string{filepath}.find_last_of('/')) + '/';

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//process all of the node meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(processMesh(mesh, scene));
	}

	//process all of the childrens meshes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	Texture2D textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//process vertices
		vertices.emplace_back(mesh->mVertices[i].x);
		vertices.emplace_back(mesh->mVertices[i].y);
		vertices.emplace_back(mesh->mVertices[i].z);

		//process normals
		vertices.emplace_back(mesh->mNormals[i].x);
		vertices.emplace_back(mesh->mNormals[i].y);
		vertices.emplace_back(mesh->mNormals[i].z);

		//process texCoords
		if (mesh->mTextureCoords[0])
		{
			vertices.emplace_back(mesh->mTextureCoords[0][i].x);
			vertices.emplace_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices.emplace_back(0.0f);
			vertices.emplace_back(0.0f);
		}
	}

	//process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//diffuse maps
		for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);

			//check if the texture was already loaded
			std::string find_path{ (directory + str.C_Str()).c_str() };
			bool isFound = false;
			for (auto& tex : loadedTextures)
			{
				for (auto& tex_data : tex.get_textures())
				{
					if (tex_data._filepath== find_path)
					{
						//textures.add_texture_data(tex_data);
						return { vertices, indices, &loadedTextures.back() };
					}
				}
			}
			if (!isFound)
			{
				textures.add_texture(find_path.c_str());
			}
		}

		//specular maps
		for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++)
		{
			aiString str;
			material->GetTexture(aiTextureType_SPECULAR, i, &str);

			//check if the texture was already loaded
			std::string find_path{ (directory + str.C_Str()).c_str() };
			bool isFound = false;
			for (auto& tex : loadedTextures)
			{
				for (auto& tex_data : tex.get_textures())
				{
					if (tex_data._filepath == find_path)
					{
						//textures.add_texture_data(tex_data);
						isFound = true;
						return { vertices, indices, &loadedTextures.back() };
					}
				}
			}
			if (!isFound)
			{
				textures.add_texture(find_path.c_str());
			}
		}
	}

	loadedTextures.push_back(textures);

	return { vertices, indices, &loadedTextures.back() };
}