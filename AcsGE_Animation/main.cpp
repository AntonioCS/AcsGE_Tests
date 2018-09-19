#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <memory>
#include <thread>

#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>

#include "AcsGE/Util/SDL_io.h"
#include "AcsGE/EventManager.h"
#include "AcsGE/Renderer.h"
#include "AcsGE/Util/ColorList.h"
#include "AcsGE/Window.h"
#include "AcsGE/Texture.h"
#include "AcsGE/Sprite.h"
#include "AcsGE/Animation.h"

/*#include "AcsGE/ECS/EntityManager.h"
#include "src/AcsGE/ECS/Components/SpriteComponent.h"
#include "src/AcsGE/ECS/Systems/RendererSystem.h"
#include "src/CustomSystems/MapSystem.h"
#include "src/GameValues.h"
#include "src/Game.h"
#include "src/CustomSystems/OverlaySystem.h"
#include "src/CustomSystems/PlayerSystem.h"*/


int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;

    ///*
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << '\n';
        std::cin.get();
        exit(-1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) == 0) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
        std::cin.get();
        exit(-1);
    }

    using AcsGameEngine::Util::ColorList;
    constexpr int windowWidth{ 1280 };
    constexpr int windowHeight{ 640 };


    AcsGameEngine::Window window{ "Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth , windowHeight };
    AcsGameEngine::Renderer renderer(window);
    AcsGameEngine::EventManager eventManager;
    /*
    constexpr int maxFrames = 16;
    const AcsGameEngine::Texture texture{ renderer.make_texture("spritesheet_walking_test.png", ColorList::white) };
    AcsGameEngine::Animation animation{
        "Test",
        texture,
        std::chrono::milliseconds{150}
    };

    int x = 0;
    int y = 0;
    int moveBy = 125;

    for (int i = 0; i < 4; i++) {
        for (int ii = 0; ii < 4; ii++) {
            animation.add({x, y, moveBy, moveBy}, std::chrono::milliseconds{ 25 });
            x += moveBy;
        }

        y += moveBy;
        x = 0;
    }*/
    /*
    const AcsGameEngine::Texture texture{ renderer.make_texture("Sprites.png") };
    AcsGameEngine::Animation animation{
        "Walking",
        texture,
        std::chrono::milliseconds{150}
    };

    std::array<SDL_Rect, 12> frames{
        {
            {8, 68, 26, 60},
            {58, 68, 25, 60},
            {109, 68, 22, 60},
            {156, 68, 22, 60},
            {198, 68, 36, 60},
            {245, 68, 39, 60},
            {297, 68, 28, 60},
            {348, 68, 24, 60},
            {397, 68, 24, 60},
            {443, 68, 25, 60},
            {486, 68, 39, 60},
            {535, 68, 36, 60}
        },
    };

    std::for_each(frames.begin(), frames.end(), [&animation](const SDL_Rect &i) {
        animation.add(i, std::chrono::milliseconds{ 155 });
    });
    */

    const AcsGameEngine::Texture texture{ renderer.make_texture("run3/adventurer-run3-sword-Sheet.png") };

    AcsGameEngine::Animation animation{
        "Running",
        texture,
        {0, 0, 50, 37},
        6,
        {50, 0,  0 ,0},
        std::chrono::milliseconds{ 155 }
    };

    AcsGameEngine::Animation animation2{
        "Running",
        texture
        //,
        //{0, 0, 50, 37},
        //6,
        //{50, 0,  0 ,0},
        //std::chrono::milliseconds{ 55 }
    };

    std::array<SDL_Rect, 6> run3frames{
        {
            {12, 8, 24, 28},
            {63, 9, 23, 27},
            {113, 11, 23, 25},
            {163, 8, 27, 28},
            {213, 9, 23, 27},
            {264, 11, 22, 25}
        }
    };

    std::for_each(run3frames.begin(), run3frames.end(), [&animation2](const SDL_Rect &i) {
        animation2.add(i, std::chrono::milliseconds{ 155 });
    });

    //using namespace AcsGameEngine::Util;

    //SDL_Rect test{ 2, 4, 5, 6 };
    //std::cout << test << '\n';

    //AcsGameEngine::Sprite sprite{ texture , { 0, 64, 45, 64 } };

    //renderer.DrawSprite(sprite);

    //renderer.Present();

    //std::this_thread::sleep_for(std::chrono::seconds(5));



    bool running{ true };

    eventManager.onQuit([&running](SDL_Event & e) {
        running = false;
    });

    eventManager.onKeyUp([&animation](SDL_Event & e) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            animation.speedUpBy(25ms);
            break;
        case SDLK_DOWN:
            animation.delayBy(25ms);
            break;
        }
    });

    // https://gist.github.com/mariobadr/673bbd5545242fcf9482
    using namespace std::chrono_literals;
    using clock = std::chrono::high_resolution_clock;

    // we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
    //constexpr std::chrono::milliseconds timeStep{ 16ms };
    constexpr std::chrono::milliseconds timeStep{ 16ms };

    std::chrono::milliseconds lag{};
    std::chrono::milliseconds frameTime{}; //deltaTime I prefer the word frameTime
    auto timeStart = clock::now();
    double angle = 0;
    // http://gameprogrammingpatterns.com/game-loop.html#play-catch-up
    while (running) {
        auto timeCurrent = clock::now();
        frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeCurrent - timeStart);
        timeStart = timeCurrent;
        //lag += frameTime;

        //std::cout << "Accumulator: " << lag.count() << "ms\n";

        try {
            eventManager.processEvents();
            renderer.Clear(ColorList::white);
            /*
            while (lag >= timeStep)
            {
                std::cout << "EXECUTED\n";
                lag -= timeStep;
            }*/

            auto &currentSprite = animation.get();
            currentSprite.setDestinationXY(650, 250);// , 100, 74);

            auto &currentSprite2 = animation2.get();
            currentSprite2.setDestinationXY(650, 300);// , 100, 74);

            //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            //SDL_RenderDrawLine(renderer, 320, 200, 300, 240);

            //currentSprite.angle(50.0);
            //std::cout << "Angle: " << currentSprite.angle() << '\n';
            //currentSprite.flip(SDL_FLIP_HORIZONTAL);
            //currentSprite.flip(SDL_FLIP_VERTICAL);
            //currentSprite.flip(static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));

           // angle = static_cast<double>(static_cast<int>(angle + 1) % 360);
            //std::cout << "Angle: " << angle << '\n';
           // currentSprite.angle(angle);

                //auto dest = currentSprite.getDestination();
                //auto source = currentSprite.getSource();
                //std::cout << "Destination X " << dest.x << " Y " << dest.y << " W " << dest.w << " H " << dest.h << '\n';
                //std::cout << "Source X " << source.x << " Y " << source.y << " W " << source.w << " H " << source.h << '\n';

            renderer.DrawSprite(currentSprite);
            renderer.DrawSprite(currentSprite2);

            renderer.DrawRect(currentSprite, ColorList::red);
            renderer.DrawRect(currentSprite2, ColorList::red);

            renderer.DrawPoint(
                currentSprite2.getDestinationPoint().first,
                currentSprite2.getDestinationPoint().second,
                ColorList::black
            );


            //renderer.DrawLine(650, 460, 700, 460, ColorList::black);

            renderer.Present();
            std::this_thread::sleep_for(timeStep);
        }
        catch (const std::exception &e) {
            std::cout << "Exception: " << e.what();
        }
        catch (...) {
            std::cout << "Error - Uncaught exception\n";
        }
    }

    IMG_Quit();
    SDL_Quit();

    return 0;
}
