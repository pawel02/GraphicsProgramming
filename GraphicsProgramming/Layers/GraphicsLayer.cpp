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
	program{ "./Shaders/vertex.glsl", "./Shaders/fragment.glsl" }
{
	//sub to all the necessary events
	EventBus::subscribe<KeyPressedEvent>([&](BasicEvent* ev) {
		return handle_key_pressed(static_cast<KeyPressedEvent*>(ev));
	});
	EventBus::subscribe<KeyReleasedEvent>([&](BasicEvent* ev) {
		return handle_key_released(static_cast<KeyReleasedEvent*>(ev));
	});
	EventBus::subscribe<MouseMovedEvent>([&](BasicEvent* ev) {
		mouse_state |= BIT(1);
		mouse_moved = static_cast<MouseMovedEvent*>(ev)->get_pos();
		return false;
	});

	//do all of the preperation
	//create the vertices and everything needed for the first triangle
	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	//   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	//   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	//};
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 5 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 5 * sizeof(float)}
		}
	});

	//add all of the textures
	Texture2D textures = Texture2D("./assets/tex1.jpg");
	textures.add_texture("./assets/tex2.jpg");

	program.set_uniform_1i("_tex1", 0);
	program.set_uniform_1i("_tex2", 1);

	program.set_uniform_1f("blend_factor", blend_factor);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	
	program.set_uniform_mat4f("projection", projection);
}

void GraphicsLayer::on_detach()
{
	//do all the necessary cleanup
}

void GraphicsLayer::on_update(float deltaTime)
{
	float speed = cameraSpeed * deltaTime;
	if (_keys_down & BIT(0)) // W
	{
		//cameraPos += speed * glm::vec3(1.0f, 0.0f, 1.0f) * cameraFront; //lock on the xz plane
		cameraPos += speed * cameraFront;
	}
	if (_keys_down & BIT(1)) // S
	{
		//cameraPos -= speed * glm::vec3(1.0f, 0.0f, 1.0f) * cameraFront;
		cameraPos -= speed * cameraFront;
	}
	if (_keys_down & BIT(2)) // A
	{
		//cameraPos -= speed * glm::vec3(1.0f, 0.0f, 1.0f) * glm::normalize(glm::cross(cameraFront, cameraUp));
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
	if (_keys_down & BIT(3)) // D
	{
		//cameraPos += speed * glm::vec3(1.0f, 0.0f, 1.0f) * glm::normalize(glm::cross(cameraFront, cameraUp));
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	//mouse movment
	if (mouse_state & BIT(1))
	{
		handle_mouse(deltaTime);
		mouse_state ^= BIT(1);
	}


	glm::mat4 view = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp
	);

	program.set_uniform_mat4f("view", view);


	//create a transformation to move the rectangle
	for (auto& cube : cubePos)
	{
		glm::mat4 model{ 1.0f };
		model = glm::translate(model, cube);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(.5f, 1.0f, 0.0f));

		program.set_uniform_mat4f("model", model);
		
		//draw the rectangle
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
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
		case GLFW_KEY_W:
		{
			_keys_down |= BIT(0);
			break;
		}
		case GLFW_KEY_S:
		{
			_keys_down |= BIT(1);
			break;
		}
		case GLFW_KEY_A:
		{
			_keys_down |= BIT(2);
			break;
		}
		case GLFW_KEY_D:
		{
			_keys_down |= BIT(3);
			break;
		}
	}

	return false;
}

bool GraphicsLayer::handle_key_released(KeyReleasedEvent* ev)
{
	switch (ev->get_code())
	{
		case GLFW_KEY_W:
		{
			_keys_down ^= BIT(0);
			break;
		}
		case GLFW_KEY_S:
		{
			_keys_down ^= BIT(1);
			break;
		}
		case GLFW_KEY_A:
		{
			_keys_down ^= BIT(2);
			break;
		}
		case GLFW_KEY_D:
		{
			_keys_down ^= BIT(3);
			break;
		}
	}
	return false;
}

bool GraphicsLayer::handle_mouse(double deltaTime)
{
	if (mouse_state & BIT(0))
	{
		mouse_state |= BIT(0);
		lastX = mouse_moved.x;
		lastY = mouse_moved.y;
	}

	float xOffset = mouse_moved.x - lastX;
	float yOffset = lastY - mouse_moved.y; //y is inverted

	lastX = mouse_moved.x;
	lastY = mouse_moved.y;

	xOffset *= sensitivity * deltaTime;
	yOffset *= sensitivity * deltaTime;

	yaw   += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	return false;
}