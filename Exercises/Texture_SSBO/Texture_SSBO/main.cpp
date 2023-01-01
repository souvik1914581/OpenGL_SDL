#include <glad/glad.h>
#include <SDL.h>
#include<iostream>
#include "ShaderProgram.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <stb_image.h>
#include "Texture.h"
#include "SSBO.h"

static constexpr unsigned WINDOW_WIDTH{ 640 };
static constexpr unsigned WINDOW_HEIGHT{ 480 };
static const std::string WINDOW_TITLE{ "GTA 7" };
static const std::string DEFAULT_TEXTURE{ "trollface.png" };


static const GLfloat vertices[]{

	//X		Y					Z						R		G		B					texture coordinates
	-0.5f,	-0.5f * float(sqrt(3)/2.0f),	0.0f,					0.1f,	0.8f,	0.8f,	0.0f,0.0f,//Lower Left
	0.5f,   -0.5f * float(sqrt(3)/2.0f),   0.0f,					0.1f,	1.0f,	0.1f,	1.0f,0.0f,//Lower right
	0.5f,	0.5f * float(sqrt(3)/2.0f),    0.0f,					0.1f,	0.1f,	1.0f,	1.0f,1.0f,//Upper right
	-0.5f,	0.5f * float(sqrt(3)/2.0f),    0.0f,					1.0f,	0.1f,	0.1f,	0.0f,1.0f //Upper left
};

static constexpr GLuint indices[]{
	0,1,3,
	1,2,3
};



int main(int argc, char** argv) {
	
	std::string textureFileName{ DEFAULT_TEXTURE };
	if (argc > 2) {
		textureFileName = argv[1];
	}
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
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
	vao1.LinkAttrib(vbo1, 0, 4, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0);
	
	//Link colors
	vao1.LinkAttrib(vbo1, 1, 4, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//Link Texture coordinates
	vao1.LinkAttrib(vbo1, 2, 4, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

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

	//Textures
	Texture twoDimTexture{textureFileName.c_str()};
	if (0 != twoDimTexture.Load()){
		std::cout << "Failed to load 2D texture \"" <<textureFileName <<"\""<<std::endl;
	}
	else {
		std::cout << "Loaded texture \"" << textureFileName << "\" of size : "<<twoDimTexture.Width()<<"x"<<twoDimTexture.Height()<< std::endl;
		
		twoDimTexture.Bind();
		//adjust the Texture settings

		//how to interpolate texture when it is upscaled or downscaled
		twoDimTexture.SetInterpolation(GL_NEAREST);

		//how should the texture be repeated in X (S axis)
		twoDimTexture.SetRepetitionX(GL_MIRRORED_REPEAT);
		//how should the texture be repeated in Y (T axis)
		twoDimTexture.SetRepetitionY(GL_MIRRORED_REPEAT);

		/*if we want to use clamp repetition
		* float flatColor[] = {1.0f,2.0f,3.0f};
		* glTexParameterfv(GL_TEXTURE_2D,GL_BORDER_COLOR,flatColor);
		*/
		twoDimTexture.Generate();

		/*Generate Mipmaps, which are smaller resolution versions of the Texture*/
		twoDimTexture.GenerateMipMap();

		//Texture already loaded, unload and unbind it
		twoDimTexture.Unload();
		twoDimTexture.Unbind();

	}
	
	//Get Uniform locations
	GLint scaleId = glGetUniformLocation(shaderProgram.ProgramId(), "scale");
	GLint pulsateId = glGetUniformLocation(shaderProgram.ProgramId(), "pulsateFactor");
	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ProgramId(), "tex0");

	SSBO ssbo1;
	shader_data shaderData{ 0 };

	////set image width and height in shaderData structure
	ssbo1.Bind();
	shaderData.posArray[0] = 1.0f;
	shaderData.posArray[1] = 2.0f;
	ssbo1.Load(&shaderData, sizeof(shaderData));
	constexpr int ssboBindPointIndex{ 2 };
	ssbo1.BindToIndex(ssboBindPointIndex);
	ssbo1.Unbind();

	bool runLoop = true;
	bool pulsateColors = false;
	float pulsateFactor = 1.0f;
	float pulsePeriodMs = 500.0f;	//500ms
	SDL_StartTextInput();

	//main loop
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
				std::cout << "pulsePeriod " << pulsePeriodMs << std::endl;
				break;
			
			}

		}//SDL event handler
		
		pulsateFactor = pulsateColors ? float(SDL_sin(SDL_GetTicks() / pulsePeriodMs)) : 1.0f;


		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		shaderProgram.Activate();
		glUniform1f(scaleId, 0.5f);
		glUniform1f(pulsateId, pulsateFactor);
		glUniform1i(tex0Uni, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		/*Bind the texture object in main loop*/
		twoDimTexture.Bind();
		vao1.Bind();
		//write to SSBO
#if 0	
		ssbo1.Bind();
		//1. find the storage block index
		GLuint block_index = 0;
		block_index = glGetProgramResourceIndex(shaderProgram.ProgramId(), GL_SHADER_STORAGE_BLOCK, "shader_data");
		
		//2.connect the shader storage block to the SSBO
		GLuint ssbo_binding_point_index = 2;
		glShaderStorageBlockBinding(shaderProgram.ProgramId(), block_index, ssbo_binding_point_index);
		
		
		ssbo1.BindToIndex(ssbo_binding_point_index);
		GLvoid* ptr = ssbo1.Map(GL_WRITE_ONLY);
		shaderData.posArray[0] = 1.0f;
		shaderData.posArray[1] = 1.0f;
		shaderData.posArray[2] = 1.0f;
		ssbo1.Unmap();
#endif
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(mainWindow);
	}

	/*exit procedure*/
	twoDimTexture.Unbind();
	twoDimTexture.Delete();
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