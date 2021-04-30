#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include "../Events/Event.h"
#include "../utils/Vectors.h"

class Window
{
public:
	using Func = std::function<void(BasicEvent&)>;

	Window(int width, int height, const char* title) noexcept;
	
	//will never call the default
	Window() = delete;
	~Window() noexcept;

	inline void set_function_callback(const Func& func) { _data._callback = func;  };
	inline bool is_window_open() const noexcept { return !glfwWindowShouldClose(_window);  };

	void close();
	void resize(const Vec2<int>& size);

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

		Func _callback;
	};

	WindowData _data;
};