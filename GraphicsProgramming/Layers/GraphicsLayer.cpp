#include <algorithm>
#include <iostream>

#include "../Core/Core.h"

#include "GraphicsLayer.h"
#include "../Textures/Texture2D.h"

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

	//cube vertices with texCoords
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31, 32, 33, 34, 35
	};

	//Create the VAO that will prepare all of the data ready for drawing
	VAO = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)}
		}
	});

	light_source = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)}
		}
	});

	_program.set_uniform_1f("blend_factor", blend_factor);
	_program.set_uniform_3f("lightColor", 1.0f, 1.0f, 1.0f);

	//add all of the necessary
	glm::mat4 model{ 1.0f };
	glm::vec3 lightpos{ 1.2f, 1.0f, 2.0f };
	model = glm::translate(model, lightpos);
	model = glm::scale(model, glm::vec3{0.3f});

	_basic_program.set_uniform_mat4f("model", model);

	_program.set_uniform_3f("lightPos", lightpos.x, lightpos.y, lightpos.z);


	//set the material for the cubes
	_program.set_uniform_3f("material.Ambient", 1.0f, 0.5f, 0.31f);
	_program.set_uniform_3f("material.Diffuse", 1.0f, 0.5f, 0.31f);
	_program.set_uniform_3f("material.Specular", 0.5f, 0.5f, 0.5f);
	_program.set_uniform_1f("material.Shininess", 256.0f);

	//set the lighting
	_program.set_uniform_3f("light.Ambient", 0.2f, 0.2f, 0.2f);
	_program.set_uniform_3f("light.Diffuse", 0.5f, 0.5f, 0.5f);
	_program.set_uniform_3f("light.Specular", 1.0f, 1.0f, 1.0f);
}

void GraphicsLayer::on_detach()
{
	//do all the necessary cleanup
}

void GraphicsLayer::on_update(float deltaTime)
{
	_camera.on_update(deltaTime);

	//create a transformation to move the rectangle
	_program.bind();
	VAO.bind();

	_program.set_uniform_3f("light.Pos", lightpos.x, lightpos.y, lightpos.z);

	glm::vec3 lightColor;
	lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

	_program.set_uniform_3f("light.Ambient", ambientColor.x, ambientColor.y, ambientColor.z);
	_program.set_uniform_3f("light.Diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);

	for (auto& cube : cubePos)
	{
		glm::mat4 model{ 1.0f };
		model = glm::translate(model, cube);

		_program.set_uniform_mat4f("model", model);
		//draw the rectangle
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	
	//draw the light source
	light_source.bind();
	_basic_program.bind();

	// move the light around the scene
	glm::mat4 model{ 1.0f };

	lightpos.x = std::sin((float)glfwGetTime()) * 5.0f;
	lightpos.z = std::cos((float)glfwGetTime()) * 5.0f;

	model = glm::translate(model, lightpos);
	model = glm::scale(model, glm::vec3{ 0.3f });

	_basic_program.set_uniform_mat4f("model", model);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
		case GLFW_KEY_UP:
		{
			blend_factor = std::clamp(blend_factor + 0.1f, 0.0f, 1.0f);
			_program.set_uniform_1f("blend_factor", blend_factor);
			break;
		}
		case GLFW_KEY_DOWN:
		{
			blend_factor = std::clamp(blend_factor - 0.1f, 0.0f, 1.0f);
			_program.set_uniform_1f("blend_factor", blend_factor);
			break;
		}
	}

	return false;
}