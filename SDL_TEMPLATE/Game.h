#pragma once
#include <SDL.h>
#define SDL_MAIN_HANDLED
#include <memory>

class GameWindow;

class Game {
private:
    Game();

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    static Game* getInstance();

private:
    bool running;
    GameWindow* gameWindow;
    SDL_Event event;

private:
    void setOpenGLAttributes();

    bool initSDL();
    bool initGLAD();

public:
    void initializeEverything();

    void input();
    void update();
    void render();

    const bool& isRunning() const;

    void clean();
};

