#include "Camera.h"
#include <SDL.h>
#include "ProgramValues.h"
#include "GameWindow.h"

Camera::Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f),
    glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
    float startYaw = -90.0f,
    float startPitch = 0.0f)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(0.05f), sensitivity(0.1f) {
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(SDL_Event& event, GameWindow* window) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                if (!ProgramValues::isLockedInPressed) {
                    ProgramValues::isLockedInPressed = true;
                    ProgramValues::isLockedIn = !ProgramValues::isLockedIn;

                    if (!ProgramValues::isLockedIn) {
                        SDL_WarpMouseInWindow(window->getWindow(), window->width() / 2, window->height() / 2);
                    }
                }
                break;
            case SDLK_w:     ProgramValues::moveForwardPressed  = true; break;
            case SDLK_a:     ProgramValues::moveLeftPressed     = true; break;
            case SDLK_s:     ProgramValues::moveBackwardPressed = true; break;
            case SDLK_d:     ProgramValues::moveRightPressed    = true; break;
            case SDLK_SPACE: ProgramValues::moveUpPressed       = true; break;
            case SDLK_LCTRL: ProgramValues::moveDownPressed     = true; break;
            case SDLK_r:     ProgramValues::sprinting           = true; break;
            case SDLK_LSHIFT:ProgramValues::fastZoom            = true; break;
            default: break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                if (ProgramValues::isLockedInPressed) {
                    ProgramValues::isLockedInPressed = false;
                }
                break;
            case SDLK_w:     ProgramValues::moveForwardPressed  = false; break;
            case SDLK_a:     ProgramValues::moveLeftPressed     = false; break;
            case SDLK_s:     ProgramValues::moveBackwardPressed = false; break;
            case SDLK_d:     ProgramValues::moveRightPressed    = false; break;
            case SDLK_SPACE: ProgramValues::moveUpPressed       = false; break;
            case SDLK_LCTRL: ProgramValues::moveDownPressed     = false; break;
            case SDLK_r:     ProgramValues::sprinting           = false; break;
            case SDLK_LSHIFT:ProgramValues::fastZoom            = false; break;
            default: break;
        }
    }
}

void Camera::processMouseMotion(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION && ProgramValues::isLockedIn) {
        float xoffset = static_cast<float>(event.motion.xrel);
        float yoffset = static_cast<float>(event.motion.yrel);

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch -= yoffset;   

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (!ProgramValues::isLockedIn)
            ProgramValues::isLockedIn = true;
    } else if (event.type == SDL_MOUSEWHEEL && ProgramValues::isLockedIn) {
        float localSensitivity =
            ProgramValues::fastZoom ? event.wheel.y * sensitivity * FOV_SPEED_MULTIPLIER : event.wheel.y * sensitivity;
        fov -= localSensitivity;

        if (fov < 1.0f) fov = 1.0f;
        if (fov > FOV_MAX) fov = FOV_MAX;
    }
}

void Camera::setViewToShader(GLuint shaderID, const std::string& uniformName) const {
    glm::mat4 view = getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaderID, uniformName.c_str()), 1, GL_FALSE, &view[0][0]);
}

void Camera::update() {
    SDL_SetRelativeMouseMode(ProgramValues::isLockedIn ? SDL_TRUE : SDL_FALSE);

    if (ProgramValues::isLockedIn) {
        float modifiedSpeed = ProgramValues::sprinting ? speed * SPRINT_MULTIPLIER : speed;

        if (ProgramValues::moveForwardPressed)  position += modifiedSpeed * front;
        if (ProgramValues::moveLeftPressed)     position -= glm::normalize(glm::cross(front, up)) * modifiedSpeed;
        if (ProgramValues::moveBackwardPressed) position -= modifiedSpeed * front;
        if (ProgramValues::moveRightPressed)    position += glm::normalize(glm::cross(front, up)) * modifiedSpeed;
        if (ProgramValues::moveUpPressed)       position += speed * up;
        if (ProgramValues::moveDownPressed)     position -= speed * up;

        updateCameraVectors();
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(dir);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

float Camera::getFOV() const {
    return fov;
}