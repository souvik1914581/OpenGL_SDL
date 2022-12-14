
#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <iostream>
#include <string>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

//Screen dimension constants
static constexpr int SCREEN_WIDTH{ 640 };
static constexpr int SCREEN_HEIGHT{ 480 };
static const std::string SCREEN_TITLE{ "GTA 7" };






/*Vertex coordinates for our equilateral triangle*/
static GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	//lower left corner	-->0
	-0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,											//inner left		-->1
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	//upper corner		-->2
	0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,	//inner right		-->3
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		//lower right corner	-->4
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f											//inner down		-->5
};

static GLuint indices[] =
{
	0,1,5,
	1,2,3,
	5,3,4
};

int main(int argc, char** argv)
{
	//init SDL

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	//set SDL OpenGL context
	//Use OpenGL 3.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window = SDL_CreateWindow(SCREEN_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	
	
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL)
	{
		std::cout << "Failed to create GL context: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	gladLoadGL();

	if (SDL_GL_SetSwapInterval(1) != 0)
	{
		std::cout << "Warning! Failed to set swap interval: " << SDL_GetError() << std::endl;
	}

	
	//set view port
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	VAO vao1;
	vao1.Bind();

	VBO vbo1(vertices, sizeof(vertices));
	EBO ebo1(indices, sizeof(indices));
	vao1.LinkVBO(vbo1, 0);
	
	//unbind
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	bool runProgram = true;
	while (runProgram)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				runProgram = false;
				break;
			case SDL_TEXTINPUT:
				const auto& inputKey = e.text.text[0];
				if (inputKey == 'q' || inputKey == 'Q')
					runProgram = false;
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		

		shaderProgram.Activate();
		glClear(GL_COLOR_BUFFER_BIT);
		vao1.Bind();
		glDrawElements(GL_TRIANGLES, 9,GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(window);
	}

	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	shaderProgram.Delete();
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
}


