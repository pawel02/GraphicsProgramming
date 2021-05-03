#include "Application.h"
#include "../Layers/GraphicsLayer.h"

Application::Application() 
	:_window{ 1200, 800, "Learning OpenGL" }
{
	//create all of the layers needed to run the applicaion
	_layers.emplace_back(std::make_unique<GraphicsLayer>(&_window));
}

Application::~Application() 
{
	for (auto& layer : _layers)
	{
		layer->on_detach();
	}

	glfwTerminate();
}

void Application::run()
{
	//create the main loop
	while (_window.is_window_open())
	{
		//handle rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//update all of the layers
		for (auto& layer : _layers)
		{
			layer->on_update();
		}

		//swap the buffers and poll for events
		glfwSwapBuffers(_window.get_window());
		glfwPollEvents();
	}
}