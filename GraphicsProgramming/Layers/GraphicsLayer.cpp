#include <algorithm>
#include <iostream>

#include "../Core/Core.h"

#include "GraphicsLayer.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GraphicsLayer::GraphicsLayer(Window* window) noexcept
	:_window{ window },
	_program{ "./Shaders/vertex.glsl", "./Shaders/fragment.glsl" },
	_basic_program{ "./Shaders/basic_vertex.glsl", "./Shaders/basic_fragment.glsl" },
	_camera{&_program, 800.0f, 600.0f}
{
	_camera.add_shader(&_basic_program);

	//sub to all the necessary events
	EventBus::subscribe<KeyPressedEvent>([&](BasicEvent* ev) {
		return handle_key_pressed(static_cast<KeyPressedEvent*>(ev));
		});

	//setup the model fragment shader
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, glm::vec3(1.0f, 1.2f, 1.0f));

	window_source = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 8 * sizeof(float)}
		}
	});

	_textures.add_texture("./assets/window.png");

	_program.set_uniform_mat4f("model", model);
	_program.set_uniform_1i("texture_diffuse1", 0);

	light_source = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 8 * sizeof(float)}
		}
	});

	//move the light to the correct position
	model = glm::mat4{ 1.0f };
	glm::vec3 lightpos{ 1.2f, 1.0f, 2.0f };
	model = glm::translate(model, lightpos);
	model = glm::scale(model, glm::vec3{0.3f});
	_basic_program.set_uniform_mat4f("model", model);
}

void GraphicsLayer::on_detach()
{
}

void GraphicsLayer::on_update(float deltaTime)
{
	//draw the window
	_program.bind();
	_textures.bindAll();
	window_source.bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//draw the light source
	light_source.bind();
	_basic_program.bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	_camera.on_update(deltaTime);
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
			if (!isPolygon) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isPolygon = !isPolygon;

			//allow other things to respond to P click
			break;
		}
	}

	return false;
}