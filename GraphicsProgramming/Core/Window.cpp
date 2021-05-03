#include "Window.h"
#include <iostream>
#include "../Events/WindowEvents.h"
#include "../Events/Keyboard.h"
#include "../Events/Mouse.h"
#include "../utils/Vectors.h"

Window::Window(int width, int height, const char* title) noexcept
	:_data{}
{
	//initialize the GLFW
	if (!glfwInit())
	{
		std::cout << "Could not initialize GLFW\n";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	
	_window = glfwCreateWindow(width, height, title, NULL, NULL);
	
	//disable the mouse
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (_window == NULL)
	{
		std::cout << "Failed to create a window\n";
		glfwTerminate();
	}
	else
	{
		glfwMakeContextCurrent(_window);

		//initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD\n";
		}
		else
		{
			glViewport(0, 0, width, height);

			//subscribe to the window events
			EventBus::subscribe<WindowResizeEvent>([&](BasicEvent* ev) {
				return resize(static_cast<WindowResizeEvent*>(ev));
			});

			EventBus::subscribe<WindowCloseEvent>([&](BasicEvent* ev) {
				return close();
			});

			register_events();
		}
	}
}

Window::~Window() noexcept
{}

void Window::register_events() noexcept
{
	//store the data pointer in glfw
	glfwSetWindowUserPointer(_window, &_data);

	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(window)));
		data._width = width;
		data._height = height;

		WindowResizeEvent ev{width, height};
		EventBus::publish<WindowResizeEvent>(&ev);
	});


	glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
		WindowCloseEvent ev{};
		EventBus::publish<WindowCloseEvent>(&ev);
	});

	//all of the keycallbacks
	glfwSetKeyCallback(_window, []
	(GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent ev{ key };
				EventBus::publish<KeyPressedEvent>(&ev);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent ev{ key };
				EventBus::publish<KeyReleasedEvent>(&ev);
				break;
			}
		}
	});

	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
		MouseMovedEvent ev{xpos, ypos};
		EventBus::publish<MouseMovedEvent>(&ev);
	});
}


bool Window::close()
{
	glfwSetWindowShouldClose(_window, true);
	return true;
}

bool Window::resize(WindowResizeEvent* ev)
{
	Vec2<int> size = ev->get_size();

	_data._width = size.x;
	_data._height = size.y;

	glViewport(0, 0, size.x, size.y);
	return true;
}