#include <algorithm>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Core.h"

#include "GraphicsLayer.h"

GraphicsLayer::GraphicsLayer(Window* window) noexcept
	:_window{ window },
	_program{ "./Shaders/vertex.glsl", "./Shaders/fragment.glsl" },
	_basic_program{ "./Shaders/basic_vertex.glsl", "./Shaders/basic_fragment.glsl" },
	cubemap_program{ "./Shaders/cubemap_vs.glsl", "./Shaders/cubemap_fs.glsl" },
	_camera{&_program, 800.0f, 600.0f}
{
	_camera.add_shader(&_basic_program);

	//sub to all the necessary events
	EventBus::subscribe<KeyPressedEvent>([&](BasicEvent* ev) {
		return handle_key_pressed(static_cast<KeyPressedEvent*>(ev));
		});

	//setup the model fragment shader
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, glm::vec3(1.0f, 1.2f, 1.0f));

	window_source = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 8 * sizeof(float)}
		}
	});

	_textures.add_texture("./assets/window.png");

	_program.set_uniform_mat4f("model", model);
	_program.set_uniform_1i("texture_diffuse1", 0);

	light_source = std::move(VertexArray<float, unsigned int>{
		vertices, sizeof(vertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 8 * sizeof(float)},
			VertexAttribLayout{2, GL_FLOAT, GL_FALSE, 8 * sizeof(float)}
		}
	});

	//move the light to the correct position
	model = glm::mat4{ 1.0f };
	glm::vec3 lightpos{ 1.2f, 1.0f, 2.0f };
	model = glm::translate(model, lightpos);
	model = glm::scale(model, glm::vec3{0.3f});
	_basic_program.set_uniform_mat4f("model", model);

	cubemap = std::move(VertexArray<float, unsigned int>{
		skyboxVertices, sizeof(skyboxVertices), indices, sizeof(indices),
		{
			VertexAttribLayout{3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)},
		}
	});
	cubemapTex = load_skybox(faces);
	cubemap_program.set_uniform_1i("skybox", 0);
}

void GraphicsLayer::on_detach()
{
}

void GraphicsLayer::on_update(float deltaTime)
{
	_camera.on_update(deltaTime);

	//draw the light source
	light_source.bind();
	_basic_program.bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//draw the window
	_program.bind();
	_textures.bindAll();
	window_source.bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//draw the cubemap
	glDepthFunc(GL_LEQUAL);
	cubemap_program.bind();

	cameraMatricies matricies = _camera.get_matricies();
	cubemap_program.set_uniform_mat4f("projection", matricies.projection);

	glm::mat4 view = glm::mat4(glm::mat3(matricies.view));
	cubemap_program.set_uniform_mat4f("view", view);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
	cubemap.bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);
}

bool GraphicsLayer::handle_key_pressed(KeyPressedEvent* ev)
{

	switch (ev->get_code())
	{
		case GLFW_KEY_ESCAPE:
		{
			_window->close();
			return true;
			break;
		}
		case GLFW_KEY_P:
		{
			//toggle polygon mode
			if (!isPolygon) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isPolygon = !isPolygon;

			//allow other things to respond to P click
			break;
		}
	}

	return false;
}

unsigned int GraphicsLayer::load_skybox(std::vector<std::string> faces)
{
	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	int width, height, nChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load cubemap";
		}
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	return texID;
}