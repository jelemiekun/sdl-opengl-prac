#include "Camera.h"
#include <SDL.h>

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

void Camera::processKeyboard(const Uint8* keystates) {
    if (keystates[SDL_SCANCODE_W])
        position += speed * front;
    if (keystates[SDL_SCANCODE_S])
        position -= speed * front;
    if (keystates[SDL_SCANCODE_A])
        position -= glm::normalize(glm::cross(front, up)) * speed;
    if (keystates[SDL_SCANCODE_D])
        position += glm::normalize(glm::cross(front, up)) * speed;
    if (keystates[SDL_SCANCODE_SPACE])
        position += speed * up;
    if (keystates[SDL_SCANCODE_LCTRL])
        position -= speed * up;
}

void Camera::processMouseMotion(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::setViewToShader(GLuint shaderID, const std::string& uniformName) const {
    glm::mat4 view = getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaderID, uniformName.c_str()), 1, GL_FALSE, &view[0][0]);
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