#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <string>
#include <glad/glad.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    Camera(glm::vec3 startPosition, glm::vec3 startUp,
        float startYaw, float startPitch);

    glm::mat4 getViewMatrix() const;

    void processKeyboard(const Uint8* keystates);

    void processMouseMotion(float xoffset, float yoffset);

    void setViewToShader(GLuint shaderID, const std::string& uniformName) const;


private:
    void updateCameraVectors();
};