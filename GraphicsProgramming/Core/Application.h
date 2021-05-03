#pragma once
#include <vector>
#include <memory>

#include "Window.h"
#include "../Layers/Layer.h"

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

