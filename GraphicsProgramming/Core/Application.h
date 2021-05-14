#pragma once
#include <vector>
#include <memory>

#include "Window.h"
#include "../Layers/Layer.h"
#include "../Shaders/Shader.h"
#include "VertexArray.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	Window _window;

	std::vector<std::unique_ptr<Layer>> _layers;

	float previous_time = 0.0f;

	//my custom framebuffer
	unsigned int FBO;
	unsigned int RBO;
	unsigned int _texture;
	Shader _program;
	VertexArray<float, unsigned int> VAO;

	//Quad
	float vertices[24] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int indices[6] = {
		0, 1, 2,
		3, 4, 5
	};
};

