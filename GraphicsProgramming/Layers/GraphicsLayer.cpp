#include "GraphicsLayer.h"
#include "../Textures/Texture2D.h"
#include <algorithm>

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

	//create VBO and VAO
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//tell openGL how to interpret the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//create the EBO
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	glGenBuffers(2, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


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
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);
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