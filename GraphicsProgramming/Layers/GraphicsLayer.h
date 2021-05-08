#pragma once
#include <glm/glm.hpp>

#include "../Shaders/Shader.h"
#include "../Core/Window.h"

#include "../Events/Keyboard.h"
#include "../Events/Mouse.h"

#include "Layer.h"

#include "../Core/VertexArray.h"
#include "Camera.h"

class GraphicsLayer : public Layer
{
public:
	GraphicsLayer(Window* window) noexcept;
	~GraphicsLayer() = default;

	virtual void on_detach() override;
	virtual void on_update(float deltaTime) override;

private:
	bool handle_key_pressed(KeyPressedEvent* ev);

private:
	VertexArray<float, unsigned int> VAO;
	VertexArray<float, unsigned int> light_source;

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

	Shader _program;
	Shader _basic_program;

	Camera _camera;

	float blend_factor = 1.0f;
	glm::vec3 lightpos{ 1.0f, 1.2f, 2.0f };

};