#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture2D.h"

Texture2D::Texture2D(const char* filepath)
{
	stbi_set_flip_vertically_on_load(true);
	add_texture(filepath);
}

Texture2D::Texture2D(const char* filepath,
	const Vec2<int> texWrap, 
	const Vec2<int> texFiltering)
{
	add_texture(filepath, texWrap, texFiltering);
}


Texture2D::~Texture2D()
{
}

void Texture2D::add_texture(const char* filepath)
{
	_data.emplace_back(TextureData{
		filepath,
		{ GL_REPEAT, GL_REPEAT },
		{ GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR }
		});

	load_texture();
}

void Texture2D::add_texture(const char* filepath, const Vec2<int> texWrap, const Vec2<int> texFiltering)
{
	_data.emplace_back(TextureData{
		filepath,
		{ texWrap },
		{ texFiltering }
		});

	load_texture();
}

void Texture2D::bindAll()
{
	for (int i = 0; i < _data.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _data[i].buffer);
	}
}

void Texture2D::load_texture()
{
	const auto data = _data.end() - 1;

	data->texData = stbi_load(data->_filepath, &data->width, &data->height, &data->nChannels, 0);
	if (data->texData != nullptr)
	{
		//load the texture into graphics card
		glGenTextures(1, &data->buffer);

		//assign this to the next texture spot
		glActiveTexture(GL_TEXTURE0 + static_cast<int>(_data.size() - 1));
		glBindTexture(GL_TEXTURE_2D, data->buffer);

		//wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, data->_texWrap.x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, data->_texWrap.y);

		//filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data->_texFiltering.x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, data->_texFiltering.y);

		GLenum format;
		if (data->nChannels == 1)
			format = GL_RED;
		else if (data->nChannels == 3)
			format = GL_RGB;
		else if (data->nChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, data->width, data->height, 0, format, GL_UNSIGNED_BYTE, data->texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		//clear the memory for the image as its on the GPU now
		stbi_image_free(data->texData);
	}
	else
	{
		std::cout << "Could not load the texture " << data->_filepath << "\n";
	}
}

