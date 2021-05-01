#pragma once
#include "../Core/Window.h"
#include "../Events/Keyboard.h"

class GraphicsLayer
{
public:
	GraphicsLayer(Window* window) noexcept;
	~GraphicsLayer() = default;

private:
	bool handle_esc_pressed(KeyPressedEvent* ev);

private:
	Window* _window;
};