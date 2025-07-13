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
            case SDLK_ESCAPE:
                if (!ProgramValues::windowFocusedPressed) {
                    ProgramValues::windowFocused = !ProgramValues::windowFocused;
                    ProgramValues::windowFocusedPressed = true;
                }
                break;
            case SDLK_w: 
                ProgramValues::Movement::FRONT = true;
                break;
            case SDLK_s: 
                ProgramValues::Movement::BACK = true;
                break;
            case SDLK_a:
                ProgramValues::Movement::LEFT = true;
                break;
            case SDLK_d:
                ProgramValues::Movement::RIGHT = true;
                break;
            case SDLK_SPACE:
                ProgramValues::Movement::UP = true;
                break;
            case SDLK_LCTRL:
                ProgramValues::Movement::DOWN = true;
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
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                ProgramValues::windowFocusedPressed = false;
                break;
            case SDLK_w:
                ProgramValues::Movement::FRONT = false;
                break;
            case SDLK_s:
                ProgramValues::Movement::BACK = false;
                break;
            case SDLK_a:
                ProgramValues::Movement::LEFT = false;
                break;
            case SDLK_d:
                ProgramValues::Movement::RIGHT = false;
                break;
            case SDLK_SPACE:
                ProgramValues::Movement::UP = false;
                break;
            case SDLK_LCTRL:
                ProgramValues::Movement::DOWN = false;
                break;
            default: break;
            }
        } else if (event.type == SDL_MOUSEWHEEL) {
            ProgramValues::Camera::fov -= event.wheel.y * ProgramValues::Camera::scrollSensitivity;

            if (ProgramValues::Camera::fov < 1.0f) ProgramValues::Camera::fov = 1.0f;
            if (ProgramValues::Camera::fov > 60.0f) ProgramValues::Camera::fov = 60.0f;
        } else if (event.type == SDL_MOUSEMOTION) {
            if (ProgramValues::windowFocused) {
                float xoffset = event.motion.xrel * ProgramValues::Camera::sensitivity;
                float yoffset = event.motion.yrel * ProgramValues::Camera::sensitivity;

                ProgramValues::Camera::yaw += xoffset;
                ProgramValues::Camera::pitch -= yoffset;

                if (ProgramValues::Camera::pitch > 89.0f)
                    ProgramValues::Camera::pitch = 89.0f;
                if (ProgramValues::Camera::pitch < -89.0f)
                    ProgramValues::Camera::pitch = -89.0f;

                glm::vec3 direction;
                direction.x = cos(glm::radians(ProgramValues::Camera::yaw)) * cos(glm::radians(ProgramValues::Camera::pitch));
                direction.y = sin(glm::radians(ProgramValues::Camera::pitch));
                direction.z = sin(glm::radians(ProgramValues::Camera::yaw)) * cos(glm::radians(ProgramValues::Camera::pitch));

                ProgramValues::Camera::cameraFront = glm::normalize(direction);
            }
        }
    }
}