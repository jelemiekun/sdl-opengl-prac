#include "Game.h"
#include "FPSManager.h"

int SDL_main(int argc, char* argv[]) {
    Game* game = Game::getInstance();

    game->initialize();

    if (game->isRunning())
        game->tutorial();

    int countFrame = 0;
    Uint32 startTime = SDL_GetTicks();

    while (game->isRunning()) {
        Uint32 frameStart = SDL_GetTicks();

        game->processInput();
        game->update();
        game->render();

        FPSManager::limitFPS(frameStart);
        FPSManager::calculateAverageFPS(countFrame, startTime);
    }

    game->reset();


    return 0;
}