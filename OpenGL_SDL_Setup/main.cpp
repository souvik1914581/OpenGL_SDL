#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <iostream>
#include <string>

constexpr int SCREEN_WIDTH{ 1080 };
constexpr int SCREEN_HEIGHT{ 720 };
static const std::string WINDOW_TITLE{ "GTA 7" };

//global variables
GLuint gProgramID			=	 0;
GLuint VAO, VBO;
//SDL GL context
SDL_GLContext gContext;


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

bool initGL() {
	bool success = true;
	gProgramID = glCreateProgram();

	/*Tell OpenGL about our viewPort*/
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//create Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);


	//set the Vertex source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//compile the Vertex shader source
	glCompileShader(vertexShader);

	//check the vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE) {
		std::cout << "Failed to compile vertex shader!" << std::endl;
		success = false;
	}
	else {
		//attach vertex shader to Program
		glAttachShader(gProgramID, vertexShader);

		//create a Fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


		//Set fragment source
		glShaderSource(fragmentShader, 1,&fragmentShaderSource, NULL);

		//compile Fragment Shader
		glCompileShader(fragmentShader);

		//Check Fragment shader for errors
		GLint fragmentShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);
		if (fragmentShaderCompiled != GL_TRUE) {
			std::cout << "Unable to compile fragment shader!" << std::endl;
			success = false;
		}
		else {
			//attach fragment shader to source
			glAttachShader(gProgramID, fragmentShader);

			//Link program
			glLinkProgram(gProgramID);

			/*we are done with the now useless shaders, can be deleted*/
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			//check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE) {
				std::cout << "Failed to link program!" << std::endl;
				success = false;
			}
			else {
				//Initialize clear color
				glClearColor(0.07f, 10.0f, 100.0f, 1.0f);

				/*Vertex coordinates for our equilateral triangle*/
				GLfloat vertices[] =
				{
					-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	//lower left corner
					0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		//lower right corner
					0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f	//upper corner
				};

				/*Generate the VAO and VBO with only 1 object each*/
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);

				/*now lets bind VAO, making it the current context VAO*/
				glBindVertexArray(VAO);
				/*	Binding in OpenGL -
				Binding an object makes the object the current object.
				Whenever a function fires and modifies the current object,
				it modifies that object aka the Binded object*/

				/*Bind the VBO specifying its a GL_ARRAY_BUFFER */
				glBindBuffer(GL_ARRAY_BUFFER, VBO);

				/*Store our vertices in the buffer*/
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				/*now lets configure VAO so that OpenGL knows how to read the VBO*/
				glVertexAttribPointer(
					0,	//index of the generic vertex attribute to be modified
					3,	//number of components per generic vertex attibute(1,2,3, or 4)
					GL_FLOAT,	//data type of each component
					GL_FALSE,	//specifies if fixed-point data should be normalized
					3 * sizeof(GLfloat),	//byte offset between consecutive generic vertex attributes
					(void*)0);	//offset of the first component of the first generic vertex attribute in the array in buffer currently bound to GL_ARRAY_BUFFER
				
				/*Enable the Vertex Attribute so that OpenGL knows how to use it*/
				glEnableVertexAttribArray(0);

				/*Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO
				 and VBO we created with a function*/
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
		}
	}
	return success;
}

void render() {
	//Clear color buffer
	/*Tell OpenGL to clear the Back Buffer colors*/
	glClearColor(0.07f, 10.0f, 100.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/*tell OpenGL which Shader program to use*/
	glUseProgram(gProgramID);

	/*Bind the VAO so that OpenGL knows how to use it*/
	glBindVertexArray(VAO);

	/*Draw the triangle using the GL_TRIANGLES primitive*/
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char* argv[]) {
	bool success = true;
	//The window
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL failed to initialize: " <<SDL_GetError()<< std::endl;
	}
	else {
		//Create window
		window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cout << "Window couldn't be initialized: " << SDL_GetError() << std::endl;
		}
		else
		{
			//specify OpenGL 3.3 Core Profile
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
			//create context
			 gContext = SDL_GL_CreateContext(window);
			if (gContext == NULL) {
				std::cout << "OpenGL context couldn't be created. SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				//Initialize GLAD
				gladLoadGL();
				

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0) {
					std::cout << "Warning: Unable to set VSync! " << SDL_GetError() << std::endl;
				}

				//Initialize OpenGL
				if (!initGL()) {
					std::cout << "Failed to init OpenGL!" << std::endl;
					success = false;
				}

				/*Render loop*/
				bool quit = false;
				
				/*Event handler*/
				SDL_Event e;
				while (!quit) {
					while (SDL_PollEvent(&e) != 0) {
						//Check event type
						quit = (e.type == SDL_QUIT);
					}
					render();
					//Update screen
					SDL_GL_SwapWindow(window);
				}
			}
		
		}
	}
	/*Delete all the objects we've created*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(gProgramID);
	
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit the SDL subsystems
	SDL_Quit();
	return 0;
}