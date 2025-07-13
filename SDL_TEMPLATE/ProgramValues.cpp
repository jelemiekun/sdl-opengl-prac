#include "ProgramValues.h"

namespace ProgramValues {
	bool windowFocused = false;
	bool windowFocusedPressed = false;
	float x = 0.0f;
	float y = 0.0f;
	float dimensionScalar = 1.0f;
	vec4 v1_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	vec4 v2_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	vec4 v3_color = { 0.0f, 0.0f, 1.0f, 1.0f };
	vec4 v4_color = { 0.6f, 0.6f, 0.6f, 1.0f };
	int ProgramDimensionX = 1080;
	int ProgramDimensionY = 720;

	namespace Camera {
		float cameraSpeed = 0.05f;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float yaw = -90.0f;
		float pitch = 0.0f;
		float sensitivity = 0.1f;

		float fov = 1.0f;
		float scrollSensitivity = 1.0f;
	}

	namespace Movement {
		bool LEFT = false;
		bool RIGHT = false;
		bool FRONT = false;
		bool BACK = false;
		bool UP = false;
		bool DOWN = false;
	}
}