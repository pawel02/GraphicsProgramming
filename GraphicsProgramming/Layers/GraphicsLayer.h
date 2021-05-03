#pragma once
#include <glm/glm.hpp>

#include "../Shaders/Shader.h"
#include "../Core/Window.h"

#include "../Events/Keyboard.h"
#include "../Events/Mouse.h"

#include "Layer.h"

#include "../Core/VertexArray.h"

class GraphicsLayer : public Layer
{
public:
	GraphicsLayer(Window* window) noexcept;
	~GraphicsLayer() = default;

	virtual void on_detach() override;
	virtual void on_update(float deltaTime) override;

private:
	bool handle_key_pressed(KeyPressedEvent* ev);
	bool handle_key_released(KeyReleasedEvent* ev);

	bool handle_mouse(double deltaTime);

private:
	VertexArray<float, unsigned int> VAO;

	glm::vec3 cubePos[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f) 
	};

	Window* _window;

	bool isPolygon = false;

	Shader program;
	float blend_factor = 1.0f;

	//all the things we need for a camera
	glm::vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
	glm::vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
	glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

	float cameraSpeed = 5.0f;

	char _keys_down = 0x00;

	//mouse movement
	char mouse_state = 0; // BIT(0) first movement, BIT(1) move_moved

	Vec2<double> mouse_moved;

	double lastX;
	double lastY;

	float sensitivity = 3.0f;

	float yaw   = -90.0f;
	float pitch = 0.0f;
};