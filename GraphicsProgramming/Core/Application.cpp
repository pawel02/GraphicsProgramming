#include "Application.h"
#include "../Layers/GraphicsLayer.h"

Application::Application() 
	:_window{ 800, 600, "Learning OpenGL" }
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	previous_time = (float)glfwGetTime();
	float deltaTime = 0.0f;
	//create the main loop
	while (_window.is_window_open())
	{
		//handle rendering
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
		glStencilMask(0x00); // enable writing to the stencil buffer

		deltaTime = (float)glfwGetTime() - previous_time;
		//update all of the layers
		for (auto& layer : _layers)
		{
			layer->on_update(deltaTime);
		}

		previous_time = (float)glfwGetTime();

		//swap the buffers and poll for events
		glfwSwapBuffers(_window.get_window());
		glfwPollEvents();
	}
}