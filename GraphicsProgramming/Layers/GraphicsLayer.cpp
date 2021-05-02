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
			return false;
			break;
		}
	}

	return false;
}