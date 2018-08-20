#include <SDL2/SDL.h>

#include <iostream>
#include <chrono>
#include <thread>

#include "AcsGE/Window.h"


int main(int argc, char *argv[])
{
	///*
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Unable to initialize SDL: " << SDL_GetError() << '\n';
		std::cin.get();
		exit(-1);
	}

	constexpr int windowWidth{ 1280 };
	constexpr int windowHeight{ 640 };	


	AcsGameEngine::Window window{ 
		"Game", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		windowWidth , 
		windowHeight 
	};

	std::this_thread::sleep_for(std::chrono::seconds(5));

	SDL_Quit();

	return 0;
}
