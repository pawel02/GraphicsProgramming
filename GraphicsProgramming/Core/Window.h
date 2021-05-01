#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include "../Events/Event.h"
#include "../Events/WindowEvents.h"

class Window
{
public:
	using Func = std::function<void(BasicEvent&)>;

	Window(int width, int height, const char* title) noexcept;
	
	//will never call the default
	Window() = delete;
	~Window() noexcept;

	inline bool is_window_open() const noexcept { return !glfwWindowShouldClose(_window);  };

	bool close();
	bool resize(WindowResizeEvent* ev);

	GLFWwindow* get_window() noexcept { return _window; }

private:
	void register_events() noexcept;

private:
	GLFWwindow* _window;

	//callback used as a starting point for all of the events (gets picked up by the application)
	struct WindowData
	{
		const char* _title;

		int _width;
		int _height;
	};

	WindowData _data;
};