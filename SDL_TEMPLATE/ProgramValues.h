#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

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
	};

	struct Object {
		float ambient;
		float diffuse;
		float specular;
		float shininess;

		glm::vec3 translate;
		glm::vec3 scale;
		float rotateDegrees;
		glm::vec3 rotate;
	};

	struct Light {
		float ambient;
		float diffuse;
		float specular;
		glm::vec3 translate;
	};

	namespace Objects {
		extern std::unique_ptr<Object> object0;
		extern std::unique_ptr<Object> object1;
	};

	namespace Lights {
		extern std::unique_ptr<Light> light0;
	}
};

