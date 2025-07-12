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
}

namespace ObjectPositions {
	glm::vec3 positions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};
}