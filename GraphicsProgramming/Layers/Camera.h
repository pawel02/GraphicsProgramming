#pragma once
#include "../Shaders/Shader.h"

#include "../Events/Keyboard.h"
#include "../Events/Mouse.h"

#include <glm/glm.hpp>


class Camera
{
public:
	Camera(Shader* program, float screen_width, float screen_height) noexcept;
	~Camera() = default;
	
	void on_update(float deltaTime);

private:
	bool handle_key_pressed(KeyPressedEvent* ev);
	bool handle_key_released(KeyReleasedEvent* ev);

	bool handle_mouse(double deltaTime);
private:
	Shader* _program;


	glm::vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
	glm::vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
	glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

	float cameraSpeed = 5.0f;

	char _keys_down = 0x00;

	//mouse movement
	char mouse_state = 0; // BIT(0) first movement, BIT(1) move_moved

	Vec2<double> mouse_moved;

	double lastX = 0.0;
	double lastY = 0.0;

	float sensitivity = 3.0f;

	float yaw = -90.0f;
	float pitch = 0.0f;
};