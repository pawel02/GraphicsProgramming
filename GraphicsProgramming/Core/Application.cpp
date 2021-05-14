#include "Application.h"
#include "../Layers/GraphicsLayer.h"
#include <stdio.h>

Application::Application() 
	:_window{ 800, 600, "Learning OpenGL" },
	_program{"./Shaders/framebuffer_vertex.glsl", "./Shaders/framebuffer_fragment.glsl"}
{
	//create all of the layers needed to run the applicaion
	_layers.emplace_back(std::make_unique<GraphicsLayer>(&_window));
	
	VAO = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 4 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 4 * sizeof(float)},
		}
	});

	//create the custom framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//generate the texture for the framebuffer
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	//bind the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

	//create a render buffer for the stencil and depth buffers
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach the renderbuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//make sure the framebuffer was created successfully
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Could not create the framebuffer\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_program.set_uniform_1i("tex", 0);
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	previous_time = (float)glfwGetTime();
	float deltaTime = 0.0f;
	//create the main loop
	while (_window.is_window_open())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		//handle rendering
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		deltaTime = (float)glfwGetTime() - previous_time;
		//update all of the layers
		for (auto& layer : _layers)
		{
			layer->on_update(deltaTime);
		}

		previous_time = (float)glfwGetTime();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		_program.bind();
		VAO.bind();
		glBindTexture(GL_TEXTURE_2D, _texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glEnable(GL_DEPTH_TEST);

		//swap the buffers and poll for events
		glfwSwapBuffers(_window.get_window());
		glfwPollEvents();
	}
}