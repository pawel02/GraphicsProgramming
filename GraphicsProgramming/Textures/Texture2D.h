#pragma once
#include <stb_image.h>
#include <vector>
#include "../utils/Vectors.h"

/*
Wrapper for the texture0
*/
class Texture2D
{
public:
	Texture2D(const char* filepath);
	Texture2D(const char* filepath, const Vec2<int> texWrap, const Vec2<int> texFiltering);
	~Texture2D();

	void add_texture(const char* filepath);
	void add_texture(const char* filepath, const Vec2<int> texWrap, const Vec2<int> texFiltering);

	void bindAll();

private:
	//loads the latest texture in the _data
	void load_texture();

private:
	struct TextureData
	{
		const char* _filepath;
		Vec2<int> _texWrap;
		Vec2<int> _texFiltering;

		unsigned int buffer;
		unsigned char* texData = nullptr;
		int width, height, nChannels;
	};

	std::vector<TextureData> _data;
};