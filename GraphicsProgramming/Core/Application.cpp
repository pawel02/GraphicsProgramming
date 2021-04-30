#include "Application.h"
#include "../Shaders/Shader.h"
#include "../Layers/GraphicsLayer.h"

Application::Application() 
	:_window{500, 500, "Learning OpenGL"}
{
	_window.set_function_callback([&](BasicEvent& ev) {
		on_event(ev);
	});

	//create all of the necessary layers
	_layers.emplace_back(new GraphicsLayer{&_window});
}

Application::~Application() 
{
	//delete all of the layers
	for (auto&& layer : _layers)
	{
		delete layer;
	}
}

void Application::run() 
{
	//do all of the preperation
	//create the vertices and everything needed for the first triangle
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top left
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f    // top right
	};

	float vertices_other[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top left
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f    // top right
	};

	//create VBO and VAO
	unsigned int VBO[2], VAO[2];
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//tell openGL how to interpret the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//create the EBO
	unsigned int EBO[2];
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	glGenBuffers(2, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	Shader program = Shader{ "./Shaders/vertex.glsl", "./Shaders/fragment.glsl" };


	//create the main loop
	//draw in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (_window.is_window_open())
	{
		//handle rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//animate the color of the rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swap the buffers and poll for events
		glfwSwapBuffers(_window.get_window());
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();
}

void Application::on_event(BasicEvent& ev)
{
	Dispatcher dispatcher(ev);
	dispatcher.dispatch<WindowCloseEvent>([&](WindowCloseEvent& ev) {
		return window_close(ev);
	});
	dispatcher.dispatch<WindowResizeEvent>([&](WindowResizeEvent& ev) {
		return window_resize(ev);
	});

	//call all of the layers in here
	/*
	*/
}

bool Application::window_close(WindowCloseEvent& ev)
{
	_window.close();

	return true;
}

bool Application::window_resize(WindowResizeEvent& ev)
{
	_window.resize(ev.get_size());
	return true;
}