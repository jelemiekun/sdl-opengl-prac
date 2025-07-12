#include "HandleEvent.h"
#include "Game.h"
#include <imgui_impl_sdl2.h>
#include "ProgramValues.h"

HandleEvent::HandleEvent() {}

HandleEvent* HandleEvent::getInstance() {
    static HandleEvent instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}

void HandleEvent::input(SDL_Event& event) {
    static float movementValue = 0.005f;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
            Game::getInstance()->setRunning(false);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w: case SDLK_UP:
                ProgramValues::y -= movementValue;
                break;
            case SDLK_s: case SDLK_DOWN:
                ProgramValues::y += movementValue;
                break;
            case SDLK_a: case SDLK_LEFT :
                ProgramValues::x += movementValue;
                break;
            case SDLK_d: case SDLK_RIGHT:
                ProgramValues::x -= movementValue;
                break;
            case SDLK_MINUS:
                if (ProgramValues::dimensionScalar >= 0)
                    ProgramValues::dimensionScalar -= movementValue;
                break;
            case SDLK_EQUALS:
                ProgramValues::dimensionScalar += movementValue;
                break;
            default: break;
            }
        } else if (event.type == SDL_MOUSEWHEEL) {
            if (event.wheel.y > 0) {
                    ProgramValues::dimensionScalar += movementValue;
            } else if (event.wheel.y < 0) {
                if (ProgramValues::dimensionScalar >= 0)
                    ProgramValues::dimensionScalar -= movementValue;
            }
        }
    }
}