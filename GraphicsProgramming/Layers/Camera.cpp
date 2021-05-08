#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Core/Core.h"

#include "Camera.h"
#include "../Events/WindowEvents.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(Shader* program, float screen_width, float screen_height) noexcept
	:mouse_moved{0.0, 0.0},
	projection{ glm::perspective(glm::radians(45.0f), screen_width / screen_height, 0.1f, 100.0f) }
{
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

	EventBus::subscribe<WindowResizeEvent>([&](BasicEvent* ev) {
		Vec2<int> size = static_cast<WindowResizeEvent*>(ev)->get_size();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(size.x / size.y), 0.1f, 100.0f);

		for (const auto& p : _programs)
		{
			p->bind();
			p->set_uniform_mat4f("projection", projection);
			p->set_uniform_3f("light.Pos", cameraPos.x, cameraPos.y, cameraPos.z);
			p->set_uniform_3f("light.direction", cameraFront.x, cameraFront.y, cameraFront.z);
			p->set_uniform_1f("light.cutOff", glm::cos(glm::radians(12.5f)));
			p->set_uniform_1f("light.outerCutOff", glm::cos(glm::radians(15.5f)));
		}
		return false;
	});

	add_shader(program);
}

void Camera::add_shader(Shader* program) noexcept
{
	_programs.emplace_back(program);
	_programs.back()->bind();
	_programs.back()->set_uniform_mat4f("projection", projection);

	_programs.back()->set_uniform_3f("light.Pos", cameraPos.x, cameraPos.y, cameraPos.z);
	_programs.back()->set_uniform_3f("light.direction", cameraFront.x, cameraFront.y, cameraFront.z);
	_programs.back()->set_uniform_1f("light.cutOff", glm::cos(glm::radians(12.5f)));
	_programs.back()->set_uniform_1f("light.outerCutOff", glm::cos(glm::radians(15.5f)));
}

void Camera::on_update(float deltaTime)
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

	for (const auto& p : _programs)
	{
		p->bind();
		p->set_uniform_mat4f("view", view);
		p->set_uniform_3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
		p->set_uniform_3f("light.Pos", cameraPos.x, cameraPos.y, cameraPos.z);
		p->set_uniform_3f("light.direction", cameraFront.x, cameraFront.y, cameraFront.z);
		p->set_uniform_1f("light.cutOff", glm::cos(glm::radians(12.5f)));
		p->set_uniform_1f("light.outerCutOff", glm::cos(glm::radians(15.5f)));
	}
}


bool Camera::handle_key_pressed(KeyPressedEvent* ev)
{

	switch (ev->get_code())
	{
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

bool Camera::handle_key_released(KeyReleasedEvent* ev)
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

bool Camera::handle_mouse(double deltaTime)
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

	yaw += xOffset;
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