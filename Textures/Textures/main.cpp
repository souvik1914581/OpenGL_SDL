#include <glad/glad.h>
#include <SDL.h>
#include<iostream>
#include "ShaderProgram.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

static constexpr unsigned WINDOW_WIDTH{ 640 };
static constexpr unsigned WINDOW_HEIGHT{ 480 };
static const std::string WINDOW_TITLE{ "GTA 7" };


static const GLfloat vertices[]{

	//X		Y					Z						R		G		B
	-0.5f,	-0.5f * float(sqrt(3)/2.0f),	0.0f,					0.1f,	0.8f,	0.8f,	//Lower Left
	0.5f,   -0.5f * float(sqrt(3)/2.0f),   0.0f,					0.1f,	1.0f,	0.1f,	//Lower right
	0.5f,	0.5f * float(sqrt(3)/2.0f),    0.0f,					0.1f,	0.1f,	1.0f,	//Upper right
	-0.5f,	0.5f * float(sqrt(3)/2.0f),    0.0f,					1.0f,	0.1f,	0.1f,	//Upper left
};

static constexpr GLuint indices[]{
	0,1,3,
	1,2,3
};

int main(int argc, char** argv) {
	
	//init SDL
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "SDL Init failed: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	SDL_Window* mainWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), 
							SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, 
							WINDOW_WIDTH, 
							WINDOW_HEIGHT,SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL);
	if (mainWindow == NULL) {
		std::cout << "SDL Failed to create window: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	//initialize OpenGL
	//set GL version, core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext glContext =  SDL_GL_CreateContext(mainWindow);
	if (glContext == NULL) {
		std::cout << "Failed to create GL context: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	if (SDL_GL_SetSwapInterval(1) < 0) {
		std::cout << "Warning! Failed to set GL Swap Interval:" << SDL_GetError() << std::endl;
	}

	gladLoadGL();

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/*Init GL stuff here*/
	VAO vao1;
	

	//bind
	vao1.Bind();
	VBO vbo1;
	EBO ebo1;
	vbo1.Load(vertices, sizeof(vertices));
	ebo1.Load(indices, sizeof(indices));

	//Link coordinates
	vao1.LinkAttrib(vbo1, 0, 4, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
	
	//Link colors
	vao1.LinkAttrib(vbo1, 1, 4, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//Unbind
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	ShaderProgram shaderProgram;
	shaderProgram.AddVertexShader("default.vert");
	shaderProgram.AddFragmentShader("default.frag");
	shaderProgram.Link();
	//check shaderProgramState
	if (shaderProgram.State() != ShaderProgramState::LINKED) {
		std::cout << "Failed to Link shaderProgram! " << std::endl;
		exit(EXIT_FAILURE);
	}
	
	GLint scaleId = glGetUniformLocation(shaderProgram.ProgramId(), "scale");
	GLint pulsateId = glGetUniformLocation(shaderProgram.ProgramId(), "pulsateFactor");
	bool runLoop = true;
	bool pulsateColors = false;
	float pulsateFactor = 1.0f;
	float pulsePeriodMs = 500.0f;	//500ms
	SDL_StartTextInput();

	while (runLoop) {

		//SDL event handler
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				runLoop = false;
				break;
			case SDL_TEXTINPUT:
				switch (e.text.text[0]) {
				case 'q':
				case 'Q':
					runLoop = false;
					break;
				case 'p':
				case 'P':
					pulsateColors = !pulsateColors;
					//std::cout << "pulse: " << pulsateColors << std::endl;
					break;
				}
			case SDL_MOUSEWHEEL:
				pulsePeriodMs += e.wheel.preciseY;
				//std::cout << "pulsePeriod " << pulsePeriodMs << std::endl;
				break;
			
			}

		}//SDL event handler
		
		pulsateFactor = pulsateColors ? float(SDL_sin(SDL_GetTicks() / pulsePeriodMs)) : 1.0f;


		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		shaderProgram.Activate();
		glUniform1f(scaleId, 0.5f);
		glUniform1f(pulsateId, pulsateFactor);
		glClear(GL_COLOR_BUFFER_BIT);
		vao1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(mainWindow);
	}

	/*exit procedure*/
	vao1.Unbind();
	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	shaderProgram.Delete();

	SDL_StopTextInput();
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;
	SDL_Quit();

	return 0;
}