#include "ProgramValues.h"

namespace ProgramValues {
	namespace KeyEvents {
		bool isLockedIn = true;
		bool isLockedInPressed = false;
		bool moveForwardPressed = false;
		bool moveBackwardPressed = false;
		bool moveLeftPressed = false;
		bool moveRightPressed = false;
		bool moveUpPressed = false;
		bool moveDownPressed = false;
		bool sprinting = false;
		bool fastZoom = false;
	}

	namespace LightSource {
		glm::vec3 ambient = glm::vec3(0.2f);
		glm::vec3 diffuse = glm::vec3(0.5f);
		glm::vec3 specular = glm::vec3(1.0f);
		glm::vec3 color = glm::vec3(1.0f);
		glm::vec3 position = { 0.0f, 2.0f, 0.0f };
		glm::vec3 scale = glm::vec3(1.0f);
		glm::vec3 rotate = glm::vec3(1.0f);
		int rotateDegrees = 0.0f;
	}

	namespace Object {
		int count = 1;
		float spacing = 1.0f;
		bool revolve = false;
		float revolveSpeed = 0.00005f;
		int shininess = 1;
		glm::vec3 ambient = glm::vec3(0.2f);
		glm::vec3 diffuse = glm::vec3(0.5f);
		glm::vec3 specular = glm::vec3(1.0f);
		glm::vec3 color = glm::vec3(1.0f);
		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		bool autoRotate = false;
		float autoRotateSpeed = 0.0005f;
		glm::vec3 rotate = glm::vec3(1.0f);
		float rotateDegrees = 0.0f;
	}
}