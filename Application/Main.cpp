#include "Engine.h"
#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

// vertices
const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f
};



const GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

int main(int argc, char** argv) {
	gme::Engine engine;
	engine.Startup();
	engine.Get<gme::Renderer>()->Create("OpenGL", 800, 600);

	gme::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	gme::SetFilePath("../resources");
	
	std::shared_ptr<gme::Program> program = engine.Get<gme::ResourceSystem>()->Get<gme::Program>("basic_program");
	std::shared_ptr<gme::Shader> vshader = engine.Get<gme::ResourceSystem>()->Get<gme::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<gme::Shader> fshader = engine.Get<gme::ResourceSystem>()->Get<gme::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	//vertex array
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create vertex buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo; //element buffer object
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//uniform
	float time = 0;
	program->SetUniform("scale",time);

	glm::vec3 tint{1.0f,1.0f,1.0f};
	program->SetUniform("tint",tint);

	bool quit = false;
	while (!quit) {
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
			break;
		default:
			break;
		}
		SDL_PumpEvents();
		engine.Update();

		time += engine.time.deltaTime;
		program->SetUniform("scale",std::sin(time));

		engine.Get<gme::Renderer>()->BeginFrame();

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		engine.Get<gme::Renderer>()->EndFrame();
		
	}

	return 0;
}