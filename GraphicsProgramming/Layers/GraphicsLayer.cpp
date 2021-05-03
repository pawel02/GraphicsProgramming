#include <algorithm>
#include <iostream>

#include "GraphicsLayer.h"
#include "../Textures/Texture2D.h"

GraphicsLayer::GraphicsLayer(Window* window) noexcept
	:_window{window},
	program{ "./Shaders/vertex.glsl", "./Shaders/fragment.glsl" }
{
	//sub to all the necessary events
	EventBus::subscribe<KeyPressedEvent>([&](BasicEvent* ev) {
		return handle_key_pressed(static_cast<KeyPressedEvent*>(ev));
	});

	//do all of the preperation
	//create the vertices and everything needed for the first triangle
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	//Create the VAO that will prepare all of the data ready for drawing
	VAO = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices), 
		{ 
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
		}
	});

	//add all of the textures
	Texture2D textures = Texture2D("./assets/tex1.jpg");
	textures.add_texture("./assets/tex2.jpg");

	program.set_uniform_1i("_tex1", 0);
	program.set_uniform_1i("_tex2", 1);

	program.set_uniform_1f("blend_factor", blend_factor);
}

void GraphicsLayer::on_detach()
{
	//do all the necessary cleanup
}

void GraphicsLayer::on_update() 
{
	//draw the rectangle
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool GraphicsLayer::handle_key_pressed(KeyPressedEvent* ev)
{

	switch (ev->get_code())
	{
		case GLFW_KEY_ESCAPE:
		{
			_window->close();
			return true;
			break;
		}
		case GLFW_KEY_P:
		{
			//toggle polygon mode
			if (isPolygon) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isPolygon = !isPolygon;

			//allow other things to respond to P click
			break;
		}
		case GLFW_KEY_UP:
		{
			blend_factor = std::clamp(blend_factor + 0.1f, 0.0f, 1.0f);
			program.set_uniform_1f("blend_factor", blend_factor);
			break;
		}
		case GLFW_KEY_DOWN:
		{
			blend_factor = std::clamp(blend_factor - 0.1f, 0.0f, 1.0f);
			program.set_uniform_1f("blend_factor", blend_factor);
			break;
		}
	}

	return false;
}