#include "HandleEvent.h"
#include "Game.h"

HandleEvent::HandleEvent() {}

HandleEvent* HandleEvent::getInstance() {
    static HandleEvent instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}

void HandleEvent::input(SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        Game::getInstance()->setRunning(false);
    }
}