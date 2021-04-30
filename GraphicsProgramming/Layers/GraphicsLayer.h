#pragma once
#include "../Core/Window.h"
#include "Layer.h"
#include "../Events/Keyboard.h"

class GraphicsLayer : public Layer
{
public:
	GraphicsLayer(Window* window) noexcept;
	~GraphicsLayer() = default;

	virtual void on_update() override;
	virtual void on_event(BasicEvent& ev) override;

private:
	void handle_esc_pressed(KeyPressedEvent& ev);

private:
	Window* _window;
};