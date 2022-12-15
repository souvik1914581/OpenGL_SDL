
#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
static constexpr int SCREEN_WIDTH{ 640 };
static constexpr int SCREEN_HEIGHT{ 480 };
static const std::string SCREEN_TITLE{ "GTA 7" };

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes rendering program and clear color
bool initGL();

//Input handler
void handleKeys(unsigned char key, int x, int y);

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//Shader loading utility programs
void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderTriangle = true;

//Graphics program
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gVAO = 0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		gWindow = SDL_CreateWindow(SCREEN_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				gladLoadGL();

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				//Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramID = glCreateProgram();

	/*Tell OpenGL about our viewPort*/
	glViewport(0, 0, SCREEN_WIDTH,SCREEN_HEIGHT);

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);


	//Set vertex source
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


		//Set fragment source
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
			success = false;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);


			//Link program
			glLinkProgram(gProgramID);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", gProgramID);
				printProgramLog(gProgramID);
				success = false;
			}
			else
			{
				
					//Initialize clear color
					glClearColor(0.f, 0.f, 0.f, 1.f);

					//VBO data
					/*Vertex coordinates for our equilateral triangle*/
					GLfloat vertices[] =
					{
						-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	//lower left corner
						0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		//lower right corner
						0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f	//upper corner
					};

					/*Generate the VAO and VBO with only 1 object each*/
					glGenVertexArrays(1, &gVAO);
					glGenBuffers(1, &gVBO);

					/*now lets bind the VAO, make the VAO the current Vertex Array Object by binding it*/
					glBindVertexArray(gVAO);

					/*Bind the VBO specifying its a GL_ARRAY_BUFFER */
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);

					/*store our vertices in the buffer*/
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

					/*now lets configure VAO so that OpenGL knows how to read the VBO*/
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

					/*Enable the Vertex Attribute so that OpenGL knows how to use it*/
					glEnableVertexAttribArray(0);

					/*Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO
					 and VBO we created with a function*/
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					
					success = true;
			
			}
		}
	}

	return success;
}

void handleKeys(unsigned char key, int x, int y)
{
	//Toggle quad
	if (key == 'q')
	{
		gRenderTriangle = !gRenderTriangle;
	}
}

void update()
{
	//No per frame update needed
}

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Render quad
	if (gRenderTriangle)
	{
		glClearColor(0.07f, 10.0f, 100.0f, 1.0f);

		//Bind program
		glUseProgram(gProgramID);
		glClear(GL_COLOR_BUFFER_BIT);

		/*Bind the VAO so that OpenGL knows how to use it*/
		glBindVertexArray(gVAO);

		/*Draw the triangle using the GL_TRIANGLES primitive*/
		glDrawArrays(GL_TRIANGLES, 0, 3);

	}
}

void close()
{
	//Deallocate program
	glDeleteProgram(gProgramID);

	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//Enable text input
		SDL_StartTextInput();

		//While application is running
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				//Handle keypress with current mouse position
				else if (e.type == SDL_TEXTINPUT)
				{
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					handleKeys(e.text.text[0], x, y);
				}
			}

			//Render quad
			render();

			//Update screen
			SDL_GL_SwapWindow(gWindow);
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}