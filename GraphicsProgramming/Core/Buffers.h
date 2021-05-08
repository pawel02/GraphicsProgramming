#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

template<typename T>
class VertexBuffer
{
public:

	VertexBuffer() = default;

	VertexBuffer(T* data, size_t data_size) noexcept 
		:_data{data}, _data_size{data_size}, _draw_method{GL_STATIC_DRAW}
	{
		glGenBuffers(1, &_buffer);
	};

	VertexBuffer(T* data, size_t data_size, int draw_method) noexcept
		:_data{ data }, _data_size{ data_size }, _draw_method{draw_method}
	{
		glGenBuffers(1, &_buffer);
	};

	~VertexBuffer() = default;

	VertexBuffer(VertexBuffer&& other) noexcept
		:_buffer{other._buffer},
		_draw_method{other._draw_method},
		_data{other._data},
		_data_size{other._data_size}
	{}

	VertexBuffer& operator=(VertexBuffer&& other) noexcept
	{
		if (this != &other)
		{
			_buffer = other._buffer;
			_draw_method = other._draw_method;
			_data = other._data;
			_data_size = other._data_size;
		}
		return *this;
	}

	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	};
	void set_data()
	{
		glBufferData(GL_ARRAY_BUFFER, _data_size, _data, _draw_method);
	}

private:
	unsigned int _buffer;

	int _draw_method;

	T* _data;
	size_t _data_size;
};

template<typename T>
class IndexBuffer 
{
public:
	IndexBuffer() = default;

	IndexBuffer(T* data, size_t data_size) noexcept 
		:_data{ data }, _data_size{ data_size }, _draw_method{GL_STATIC_DRAW}
	{
		glGenBuffers(1, &_buffer);
	}

	IndexBuffer(T* data, size_t data_size, int draw_method) noexcept
		:_data{ data }, _data_size{ data_size }, _draw_method{ draw_method }
	{
		glGenBuffers(1, &_buffer);
	}

	~IndexBuffer() = default;

	IndexBuffer(IndexBuffer&& other) noexcept
		:_buffer{ other._buffer },
		_draw_method{ other._draw_method },
		_data{ other._data },
		_data_size{ other._data_size }
	{}

	IndexBuffer& operator=(IndexBuffer&& other) noexcept
	{
		if (this != &other)
		{
			_buffer = other._buffer;
			_draw_method = other._draw_method;
			_data = other._data;
			_data_size = other._data_size;
		}
		return *this;
	}

	void bind() 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
	}

	void set_data()
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _data_size, _data, _draw_method);
	}
private:
	unsigned int _buffer;

	int _draw_method;

	T* _data;
	size_t _data_size;
};