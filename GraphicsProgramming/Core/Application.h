#pragma once
#include <vector>

#include "../Events/WindowEvents.h"
#include "Window.h"

#include "../Layers/Layer.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void on_event(BasicEvent& ev);

	bool window_resize(WindowResizeEvent& ev);
	bool window_close(WindowCloseEvent& ev);

private:
	Window _window;

	std::vector<Layer*> _layers;
};

