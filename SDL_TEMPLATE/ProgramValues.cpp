#include "ProgramValues.h"

namespace ProgramValues {
	float x = 0.0f;
	float y = 0.0f;
	float dimensionScalar = 1.0f;
	vec4 v1_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	vec4 v2_color = { 0.0f, 1.0f, 0.0f, 1.0f };
	vec4 v3_color = { 0.0f, 0.0f, 1.0f, 1.0f };
	vec4 v4_color = { 0.6f, 0.6f, 0.6f, 1.0f };
	int ProgramDimensionX = 1080;
	int ProgramDimensionY = 720;

	namespace Camera{
		float cameraSpeed = 0.05f;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
}