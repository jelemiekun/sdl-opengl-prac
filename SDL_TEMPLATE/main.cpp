#include "Game.h"

int SDL_main(int argc, char* argv[]) {
    Game* game = Game::getInstance();

    game->initialize();

    if (game->isRunning())
        game->tutorial();


    while (game->isRunning()) {
        game->processInput();
        game->update();
        game->render();
    }

    game->reset();


    return 0;
}