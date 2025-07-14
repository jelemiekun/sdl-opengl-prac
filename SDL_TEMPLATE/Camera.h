#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <string>
#include <glad/glad.h>

class GameWindow;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    constexpr static float SPRINT_MULTIPLIER = 3.0f;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    Camera(glm::vec3 startPosition, glm::vec3 startUp,
        float startYaw, float startPitch);

    glm::mat4 getViewMatrix() const;

    void processKeyboard(SDL_Event& event, GameWindow* window);

    void processMouseMotion(SDL_Event& event);

    void setViewToShader(GLuint shaderID, const std::string& uniformName) const;

    void update();


private:
    void updateCameraVectors();
};