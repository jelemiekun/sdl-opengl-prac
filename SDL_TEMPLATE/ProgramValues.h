#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace ProgramValues {
	namespace KeyEvents {
		extern bool isLockedIn;
		extern bool isLockedInPressed;
		extern bool moveForwardPressed;
		extern bool moveBackwardPressed;
		extern bool moveLeftPressed;
		extern bool moveRightPressed;
		extern bool moveUpPressed;
		extern bool moveDownPressed;
		extern bool sprinting;
		extern bool fastZoom;
	}

	namespace LightSource {
		extern glm::vec3 ambient;
		extern glm::vec3 diffuse;
		extern glm::vec3 specular;
		extern glm::vec3 color;
		extern glm::vec3 position;
		extern glm::vec3 scale;
		extern glm::vec3 rotate;
		extern int rotateDegrees;
	}

	namespace Object {
		extern int count;
		extern float spacing;
		extern bool revolve;
		extern float revolveSpeed;
		extern int shininess;
		extern glm::vec3 ambient;
		extern glm::vec3 diffuse;
		extern glm::vec3 specular;
		extern glm::vec3 color;
		extern glm::vec3 position;
		extern glm::vec3 scale;
		extern bool autoRotate;
		extern float autoRotateSpeed;
		extern glm::vec3 rotate;
		extern float rotateDegrees;
	}
};

