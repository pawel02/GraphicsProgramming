#pragma once
#include "Buffers.h"
#include <initializer_list>

/*
Describes the layout of the vertex buffer
*/
struct VertexAttribLayout
{
	int size;
	int type;
	int normalized;
	int stride;

	// utility function to help get the size of GL_FLOAT and other types
	const unsigned int get_type_size() const
	{
		switch (type)
		{
			case GL_FLOAT:
			{
				return 4;
				break;
			}

			default:
			{
				return 4;
				break;
			}
		}
	}
};
/*
Stores all of the buffers needed to create any sort of shape by managing
The vertex and index buffers
*/
template<typename VertexType, 
	typename IndexType>
class VertexArray
{
public:
	VertexArray() noexcept 
		:_vertex_buffer{}, _index_buffer{}
	{};

	VertexArray(VertexType* vertices, int vertices_size, IndexType* index_data, int index_size,
		std::initializer_list<VertexAttribLayout> vertex_layouts) noexcept
		:_vertex_buffer{ vertices, vertices_size }, _index_buffer{index_data, index_size}
	{
		//create the vertex array and bind the buffers
		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

		_vertex_buffer.bind();
		_vertex_buffer.set_data();

		//set the layout of the vertex attribs
		unsigned int i = 0;
		unsigned int pointer = 0;
		for (auto& layout : vertex_layouts)
		{
			glVertexAttribPointer(i, layout.size, layout.type, layout.normalized, layout.stride, (void*)pointer);
			pointer += layout.size * layout.get_type_size();
			glEnableVertexAttribArray(i);
			++i;
		}

		_index_buffer.bind();
		_index_buffer.set_data();
	}

	// I don't really want these and can't really think of a use case at the moment
	VertexArray(const VertexArray<VertexType, IndexType>& other) = delete;
	VertexArray<VertexType, IndexType>& operator=(const VertexArray<VertexType, IndexType>& other) = delete;
	VertexArray(VertexArray<VertexType, IndexType>&& other) = delete;

	VertexArray<VertexType, IndexType>& operator=(VertexArray<VertexType, IndexType>&& other) noexcept
	{
		if (this != &other)
		{
			vertex_array = other.vertex_array;
			_vertex_buffer = std::move(other._vertex_buffer);
			_index_buffer = std::move(other._index_buffer);


			// set this to the max to make sure that you don't delete the 
			// VertexArray when the temp object gets destroyed
			other.vertex_array = 4294967295;
		}

		return *this;
	}

	~VertexArray() noexcept
	{
		if (vertex_array != 4294967295)
		{
			glDeleteVertexArrays(1, &vertex_array);
		}
	};

	void bind() 
	{
		glBindVertexArray(vertex_array);
	}

private:
	unsigned int vertex_array;

	VertexBuffer<VertexType> _vertex_buffer;
	IndexBuffer<IndexType> _index_buffer;
};

