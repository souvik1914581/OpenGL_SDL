#include <SDL.h>
#include <gl/glew.h>
#include <iostream>

constexpr int SCREEN_WIDTH{ 1080 };
constexpr int SCREEN_HEIGHT{ 720 };
static const std::string WINDOW_TITLE{ "GTA 7" };

int main(int argc, char* argv[]) {
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
			//Get Window Surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFE));

			//Update the surface
			SDL_UpdateWindowSurface(window);
			SDL_Event e;
			bool quit = false;
			while (quit == false) {
				while (SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT)
						quit = true;
				}
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit the SDL subsystems
	SDL_Quit();
	return 0;
}