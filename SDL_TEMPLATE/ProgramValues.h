#pragma once
#include <glm/glm.hpp>

struct vec4 {
	float r;
	float g;
	float b;
	float a;
};

struct vec3 {
	float x;
	float y;
	float z;
};

namespace ProgramValues {
	extern bool windowFocused;
	extern bool windowFocusedPressed;
	extern float x;
	extern float y;
	extern float dimensionScalar;
	extern vec4 v1_color;
	extern vec4 v2_color;
	extern vec4 v3_color;
	extern vec4 v4_color;
	extern int ProgramDimensionX;
	extern int ProgramDimensionY;
	
	namespace Camera {
		extern float cameraSpeed;
		extern glm::vec3 cameraPos;
		extern glm::vec3 cameraFront;
		extern glm::vec3 cameraUp;

		extern float yaw;
		extern float pitch;
		extern float sensitivity;

		extern float fov;
		extern float scrollSensitivity;
	}

	namespace Movement {
		extern bool LEFT;
		extern bool RIGHT;
		extern bool FRONT;
		extern bool BACK;
		extern bool UP;
		extern bool DOWN;
	}
}