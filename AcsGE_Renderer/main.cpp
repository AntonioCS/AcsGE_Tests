#include <SDL2/SDL.h>

#include <iostream>
#include <chrono>
#include <thread>

#include "AcsGE/Renderer.h"
#include "AcsGE/Window.h"
#include "AcsGE/Util/ColorList.h"


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
		"Renderer test",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth ,
		windowHeight
	};

	AcsGameEngine::Renderer r{ window };
	//Transparency
	auto c = AcsGameEngine::Util::ColorList::red;
	std::cout << "Alpha: " << c.a << '\n';

	r.DrawRect({ 20,20,100,100 }, c, true);


	c.a = 50;
	r.DrawRect({320,20,100,100}, c, true);

	c.alphaPercentage(50);
	r.DrawRect({20,320,100,100}, c, true);



	r.Present();
	
	std::this_thread::sleep_for(std::chrono::seconds(5));

	SDL_Quit();

	return 0;
}
