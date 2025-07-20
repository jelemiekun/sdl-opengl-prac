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

	namespace Objects {
		std::unique_ptr<Object> object0 = [] {
			auto obj = std::make_unique<Object>();
			obj->ambient = 0.2f;
			obj->diffuse = 0.5f;
			obj->specular = 1.0f;
			obj->shininess = 32.0f;
			obj->translate = glm::vec3(0.0f, 0.0f, 0.0f);
			obj->scale = glm::vec3(1.0f, 1.0f, 1.0f);
			obj->rotateDegrees = 0.0f;
			obj->rotate = glm::vec3(0.0f, 1.0f, 0.0f);
			return obj;
			}();

		std::unique_ptr<Object> object1 = [] {
			auto obj = std::make_unique<Object>();
			obj->ambient = 0.2f;
			obj->diffuse = 0.5f;
			obj->specular = 1.0f;
			obj->shininess = 32.0f;
			obj->translate = glm::vec3(0.0f, 2.5f, 0.0f);
			obj->scale = glm::vec3(1.0f, 1.0f, 1.0f);
			obj->rotateDegrees = 0.0f;
			obj->rotate = glm::vec3(0.0f, 1.0f, 0.0f);
			return obj;
			}();
	}

	namespace Lights {
		std::unique_ptr<Light> light0 = [] {
			auto obj = std::make_unique<Light>();
			obj->ambient = 0.2f;
			obj->diffuse = 0.5f;
			obj->specular = 1.0f;
			obj->translate = glm::vec3(0.0f, 5.0f, 0.0f);
			return obj;
			}();
	}
}