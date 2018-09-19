#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <thread>

#include <exception>
#include <typeinfo>
#include <string>

#include "AcsGE/Renderer.h"
#include "AcsGE/Window.h"
#include "AcsGE/EventManager.h"
#include "AcsGE/Util/ColorList.h"

#include "AcsGE/Util/Drawer.h"
#include "AcsGE/Util/Shapes/Circle.h"
#include "AcsGE/Util/Shapes/Rectangle.h"
#include "AcsGE/Util/Shapes/Triangle.h"
#include "AcsGE/Util/Shapes/Line.h"

#include "AcsGE/Util/CollisionDetection.h"

float fromChronoMsToFloat(std::chrono::milliseconds time)
{    
    auto res = std::chrono::duration_cast<std::chrono::duration<float>>(time);
    return res.count();
}

float Approach(float goal, float current, std::chrono::milliseconds dt)
{
    //const auto time = fromChronoMsToFloat(dt);
    //return current * (1 - time) + goal * time;
    ///*
    const auto diff = goal - current;
    const auto time = fromChronoMsToFloat(dt);

    std::cout << "Time: " << time << '\n';

    if (diff > time)
    {
        std::cout << "";
        return current + time;
    }

    if (diff < -time)
    {
        return current - time;
    }

    return goal;
    //*/
}

void update(AcsGameEngine::Util::Shapes::Rectangle &rect, std::chrono::milliseconds dt)
{
    //rect.origin.x = 
}

int main(int argc, char *argv[])
{

    using namespace std::chrono_literals;

    using AcsGameEngine::Renderer;
    using AcsGameEngine::Window;
    using AcsGameEngine::EventManager;
    using AcsGameEngine::Util::ColorList;
    using AcsGameEngine::Util::Vector2D;
    using AcsGameEngine::Util::Shapes::Circle;
    using AcsGameEngine::Util::Shapes::Radius;
    using AcsGameEngine::Util::Shapes::Rectangle;
    using AcsGameEngine::Util::Shapes::Triangle;
    using AcsGameEngine::Util::Shapes::Line;
    using AcsGameEngine::Util::CollisionDetection;
    using AcsGameEngine::Util::Drawer;
    
    using clock = std::chrono::high_resolution_clock;

    ///*
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << '\n';
        std::cin.get();
        exit(-1);
    }

    constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) == 0) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
        std::cin.get();
        exit(-1);
    }
    
    constexpr int windowWidth{ 1280 };
    constexpr int windowHeight{ 640 };

    Window window{ "Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth , windowHeight };
    Renderer renderer(window);
    EventManager eventManager;

    bool running{ true };

    eventManager.onQuit([&running](SDL_Event & e) {
        running = false;
    });

    // https://gist.github.com/mariobadr/673bbd5545242fcf9482        

    // we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
    //constexpr std::chrono::milliseconds timeStep{ 16ms };
    constexpr std::chrono::milliseconds timeStep{ 16ms };

    std::chrono::milliseconds lag{};
    std::chrono::milliseconds frameTime{}; //deltaTime I prefer the word frameTime
    auto timeStart = clock::now();
    double angle = 0;
    
    Vector2D pos{ 500, 300 };

    Rectangle shape{ {pos}, {100, 100} };
    shape.borderColor = ColorList::black;

    //Circle shape{circlePos};
    //shape.radius = Radius{ 150 };    
    //shape.borderColor = ColorList::black;

    Rectangle rect{ {600, 300}, {250, 250} };
    rect.borderColor = ColorList::blue;

    Triangle tri{ {100, 400}, {200,200}, {200,400} };

    Circle circle2{ {200,400}, {20} };

    Drawer draw(renderer);

    CollisionDetection colDetect;
    struct 
    {
        bool up{false};
        bool down{ false };
        bool left{ false };
        bool right{ false };
    } rectMovement;

/*
    eventManager.onKeyDown([&shape](SDL_Event & e) {
        const Vector2D vertical{ 0,20 };
        const Vector2D horizontal{ 20,0 };
        float radius = 20;

        switch (e.key.keysym.sym) {
        case SDLK_UP:
            shape.origin -= vertical;
            break;
        case SDLK_DOWN:
            shape.origin += vertical;
            break;
        case SDLK_LEFT:
            shape.origin -= horizontal;
            break;
        case SDLK_RIGHT:
            shape.origin += horizontal;
            break;
        case SDLK_KP_MINUS:
            //circle.radius -= radius;
            break;
        case SDLK_KP_PLUS:
            //circle.radius += radius;
            break;
        }        
    });
    */
    eventManager.onKeyDown([&rectMovement](SDL_Event & e)
    {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            rectMovement.up = true;
            break;
        case SDLK_DOWN:
            rectMovement.down = true;
            break;
        case SDLK_LEFT:
            rectMovement.left = true;
            break;
        case SDLK_RIGHT:
            rectMovement.right = true;
            break;
        }
        
    });
    eventManager.onKeyUp([&rectMovement](SDL_Event & e) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            rectMovement.up = false;
            break;
        case SDLK_DOWN:
            rectMovement.down = false;
            break;
        case SDLK_LEFT:
            rectMovement.left = false;
            break;
        case SDLK_RIGHT:
            rectMovement.right = false;
            break;
        }
    });

    
    rect.innerColor = ColorList::red.alphaPercentage(25);

    circle2.borderColor = ColorList::red;
    circle2.innerColor = ColorList::blue;

    Line line{ {50, 50}, {500, 100}};
    line.borderColor = ColorList::red;

    Line line2{ {50, 300}, {500, 175} };
    line.borderColor = ColorList::banana;

    Vector2D velocity{ 0, 0 };
    Vector2D movement{ 0, 0 };
    const Vector2D gravity{ 0, 50 };

    float velocityGoal = 10;

    ///*
    while (running) {
        auto timeCurrent = clock::now();
        frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeCurrent - timeStart);
        timeStart = timeCurrent;
        //lag += frameTime;

        //render

        try {
            eventManager.processEvents();
            renderer.Clear(ColorList::white);
            const auto t = fromChronoMsToFloat(timeStep);
            const float velGoalY = (rectMovement.up ? -50.f : rectMovement.down ? 50.f : 0.f);
            const float velGoalX = (rectMovement.left ? -50.f : rectMovement.right ? 50.f : 0.f);
            std::cout << velGoalX << " - " << velGoalY << '\n';
            std::cout << velocity.x << " - " << velocity.y << '\n';

            velocity.y = Approach(velGoalY, velocity.y, timeStep * 55);
            velocity.x = Approach(velGoalX, velocity.x, timeStep * 55);

            std::cout << velocity.x << " - " << velocity.y << '\n';
            std::cout << velGoalX << " - " << velGoalY << '\n';

            shape.origin += velocity * t;
            velocity += gravity * t;
            std::cout << "Velocity: " << velocity.x << ", " << velocity.y << '\n';
            
         
            rect.fill = colDetect.checkCollision(shape, rect);
            circle2.fill = colDetect.checkCollision(shape, circle2);
                    
            draw.DrawShape(rect);
            draw.DrawShape(shape);
            draw.DrawShape(shape);
            draw.DrawShape(circle2);
            draw.DrawShape(tri);

            //draw.DrawLineCustom(line); <-- NOT WORKING
            draw.DrawLine(line);
            draw.DrawLine(line2);

            //colDetect.checkCollision(line, line2);

            //renderer.DrawCircle({ 300, 200 }, 40, ColorList::black);
            //renderer.DrawCircle({ 500, 300 }, 50, ColorList::blue, true);

            //renderer.DrawPoint(500, 300, ColorList::black);
            //renderer.DrawLine(500, 300, 600, 300);

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
    //*/
    IMG_Quit();
    SDL_Quit();

    return 0;
}
