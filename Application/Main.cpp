#include "Engine.h"
#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};



const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
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

	////vertex array
	//GLuint vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	////create vertex buffer
	//GLuint vbo;
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//GLuint ebo; //element buffer object
	//glGenBuffers(1,&ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	////position
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	//glEnableVertexAttribArray(0);
	////color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	////uv
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	std::shared_ptr<gme::VertexIndexBuffer> vertexBuffer = engine.Get<gme::ResourceSystem>()->Get<gme::VertexIndexBuffer>("vertex_index_buffer");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(float), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(float), (3 * sizeof(float)));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(float), (6 * sizeof(float)));

	//texture
	gme::Texture texture;
	texture.CreateTexture("textures/llama.jpg");
	texture.Bind();

	//uniform
	float time = 0;
	
	// matrix uniform
	glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 2 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
	program->SetUniform("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	program->SetUniform("projection", projection);

	glm::vec3 translate{0.0f};
	float angle = 0;
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
		angle += engine.time.deltaTime;

		if (engine.Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == gme::InputSystem::eKeyState::Held) {
			translate.x -= 3.0f * engine.time.deltaTime;
		}
		if (engine.Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == gme::InputSystem::eKeyState::Held) {
			translate.x += 3.0f * engine.time.deltaTime;
		}
		if (engine.Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == gme::InputSystem::eKeyState::Held) {
			translate.y += 3.0f * engine.time.deltaTime;
		}
		if (engine.Get<gme::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == gme::InputSystem::eKeyState::Held) {
			translate.y -= 3.0f * engine.time.deltaTime;
		}

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, translate);
		model = glm::rotate(model, angle, glm::vec3{ 0, 1, 0 });
		model = glm::scale(model, glm::vec3{ 0.25f });
		program->SetUniform("model", model);


		engine.Get<gme::Renderer>()->BeginFrame();

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		vertexBuffer->Draw(GL_TRIANGLES);

		engine.Get<gme::Renderer>()->EndFrame();
		
	}

	return 0;
}