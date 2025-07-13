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
    static constexpr float movementValue = 0.0002f;
    static constexpr float scalarValue = 0.05f;
    static constexpr float cameraValue = 0.5f;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
            Game::getInstance()->setRunning(false);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w: 
                ProgramValues::Camera::cameraPos += ProgramValues::Camera::cameraSpeed * ProgramValues::Camera::cameraFront;
                break;
            case SDLK_s: 
                ProgramValues::Camera::cameraPos -= ProgramValues::Camera::cameraSpeed * ProgramValues::Camera::cameraFront;
                break;
            case SDLK_a:
                ProgramValues::Camera::cameraPos -= glm::normalize(
                    glm::cross(ProgramValues::Camera::cameraFront, ProgramValues::Camera::cameraUp)) 
                    * ProgramValues::Camera::cameraSpeed;
                break;
            case SDLK_d:
                ProgramValues::Camera::cameraPos += glm::normalize(
                    glm::cross(ProgramValues::Camera::cameraFront, ProgramValues::Camera::cameraUp)) 
                    * ProgramValues::Camera::cameraSpeed;
                break;
            case SDLK_SPACE:
                ProgramValues::Camera::cameraPos += ProgramValues::Camera::cameraUp * ProgramValues::Camera::cameraSpeed;
                break;
            case SDLK_LCTRL:
                ProgramValues::Camera::cameraPos -= ProgramValues::Camera::cameraUp * ProgramValues::Camera::cameraSpeed;
                break;
            case SDLK_MINUS:
                if (ProgramValues::dimensionScalar >= scalarValue)
                    ProgramValues::dimensionScalar -= scalarValue;
                break;
            case SDLK_EQUALS:
                ProgramValues::dimensionScalar += scalarValue;
                break;
            case SDLK_UP:
                ProgramValues::y -= movementValue;
                break;
            case SDLK_DOWN:
                ProgramValues::y += movementValue;
                break;
            case SDLK_LEFT:
                ProgramValues::x += movementValue;
                break;
            case SDLK_RIGHT:
                ProgramValues::x -= movementValue;
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