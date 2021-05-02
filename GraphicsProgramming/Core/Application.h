#pragma once
#include "Window.h"
#include "../Shaders/Shader.h"
#include "../Events/WindowEvents.h"

#include <vector>
#include "../Layers/Layer.h"
#include <memory>

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	Window _window;

	std::vector<std::unique_ptr<Layer>> _layers;
};

