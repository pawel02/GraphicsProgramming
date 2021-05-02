#include "Application.h"
#include "../Shaders/Shader.h"
#include "../Textures/Texture2D.h"


Application::Application() 
	:_window{ 500, 500, "Learning OpenGL" }, _graphics{&_window}
{}

Application::~Application() 
{
}

void Application::run()
{
	//do all of the preperation
	//create the vertices and everything needed for the first triangle
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	//create VBO and VAO
	unsigned int VBO[2], VAO[2];
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//tell openGL how to interpret the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
	
	//add all of the textures
	Texture2D textures = Texture2D("./assets/tex1.jpg");
	textures.add_texture("./assets/tex2.jpg");

	program.set_uniform_1i("texture1", 0);
	program.set_uniform_1i("texture2", 1);

	//create the main loop
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

	//do all the necessary cleanup
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);

	glfwTerminate();
}
