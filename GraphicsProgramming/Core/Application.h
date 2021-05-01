#pragma once
#include <vector>

#include "../Events/WindowEvents.h"
#include "Window.h"
#include "../Layers/GraphicsLayer.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	Window _window;

	GraphicsLayer _graphics;
};

