#pragma once
#include "../Shaders/Shader.h"
#include "../Core/Window.h"
#include "../Events/Keyboard.h"
#include "Layer.h"

class GraphicsLayer : public Layer
{
public:
	GraphicsLayer(Window* window) noexcept;
	~GraphicsLayer() = default;

	virtual void on_detach() override;
	virtual void on_update() override;

private:
	bool handle_key_pressed(KeyPressedEvent* ev);

private:
	unsigned int VBO[2], VAO[2];
	unsigned int EBO[2];

	Window* _window;

	bool isPolygon = false;

	Shader program;
	float blend_factor = 1.0f;
};