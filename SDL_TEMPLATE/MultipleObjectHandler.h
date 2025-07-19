#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace MultipleObjectHandler {
	extern glm::vec3 obtainPosition(const int& index, const float& spacing);
	extern glm::vec3 scatter(const int& index, const float& value);
	extern glm::vec3 revolve(const glm::vec3& position, const float& totalAngle);
};

