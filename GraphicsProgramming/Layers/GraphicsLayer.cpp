#include "GraphicsLayer.h"

GraphicsLayer::GraphicsLayer(Window* window) noexcept
	:_window{window}
{}

void GraphicsLayer::on_update() 
{
}

void GraphicsLayer::on_event(BasicEvent& ev) 
{
	//close the window if the user presses the esc
	Dispatcher dispatcher{ev};

	dispatcher.dispatch<KeyPressedEvent>([&](KeyPressedEvent& ev) {
		if (ev.get_code() == GLFW_KEY_ESCAPE)
		{
			handle_esc_pressed(ev);
			return true;
		}
		return false;
	});
}

void GraphicsLayer::handle_esc_pressed(KeyPressedEvent& ev)
{
	_window->close();
}