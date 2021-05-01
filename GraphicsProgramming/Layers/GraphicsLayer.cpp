#include "GraphicsLayer.h"

GraphicsLayer::GraphicsLayer(Window* window) noexcept
	:_window{window}
{
	EventBus::subscribe<KeyPressedEvent>([&](BasicEvent* ev) {
		return handle_esc_pressed(static_cast<KeyPressedEvent*>(ev));
	});
}


bool GraphicsLayer::handle_esc_pressed(KeyPressedEvent* ev)
{
	if (ev->get_code() == GLFW_KEY_ESCAPE)
	{
		_window->close();
		return true;
	}

	return false;
}