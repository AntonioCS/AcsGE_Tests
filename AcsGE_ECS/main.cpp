#include "AcsGE/Game.h"
#include <iostream>
#include <utility> //std::pair

#include "MainState.h"

int main(int argc, char *argv[])
{
    AcsGameEngine::Game game{"Test", {0, 0}, {1200, 800}};

    game.gsm.addState<MainState>("FirstState");


    try {
        game.Init();
        game.Run();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    game.Cleanup();

    return 0;
}