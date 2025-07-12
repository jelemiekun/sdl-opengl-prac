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
    static constexpr float movementValue = 0.0005f;
    static constexpr float scalarValue = 0.05f;
    static constexpr float cameraValue = 0.5f;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
            Game::getInstance()->setRunning(false);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w: 
                ProgramValues::y -= movementValue;
                break;
            case SDLK_s: 
                ProgramValues::y += movementValue;
                break;
            case SDLK_a:
                ProgramValues::x += movementValue;
                break;
            case SDLK_d:
                ProgramValues::x -= movementValue;
                break;
            case SDLK_MINUS:
                if (ProgramValues::dimensionScalar >= scalarValue)
                    ProgramValues::dimensionScalar -= scalarValue;
                break;
            case SDLK_EQUALS:
                ProgramValues::dimensionScalar += scalarValue;
                break;
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            case SDLK_LEFT:
                break;
            case SDLK_RIGHT:
                break;
            default: break;
            }
        } else if (event.type == SDL_MOUSEWHEEL) {
            if (event.wheel.y > 0) {
                    ProgramValues::dimensionScalar += scalarValue;
            } else if (event.wheel.y < 0) {
                if (ProgramValues::dimensionScalar >= scalarValue)
                    ProgramValues::dimensionScalar -= scalarValue;
            }
        }
    }
}