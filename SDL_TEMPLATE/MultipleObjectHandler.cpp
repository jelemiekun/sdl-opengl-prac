#include "MultipleObjectHandler.h"
#include "ProgramValues.h"

namespace MultipleObjectHandler {
    glm::vec3 obtainPosition(const int& index, const float& spacing) {
        int total = ProgramValues::Object::count;

        float offset = 2.0f / total;
        float increment = glm::pi<float>() * (3.0f - glm::sqrt(5.0f)); // Golden angle

        float y = ((index * offset) - 1.0f) + (offset / 2.0f);
        float r = glm::sqrt(1.0f - y * y);
        float phi = index * increment;

        float x = glm::cos(phi) * r;
        float z = glm::sin(phi) * r;

        // 💡 Scaling the sphere size using spacing
        return glm::vec3(x, y, z) * spacing;
    }

    glm::vec3 scatter(const int& index, const float& value) {
        // Spread objects in a circle on XZ plane
        float angle = glm::radians(360.0f / ProgramValues::Object::count * index);
        float radius = value;

        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        return glm::vec3(x, 0.0f, z); // y=0 -> flat scatter
    }

    glm::vec3 revolve(const glm::vec3& position, const float& totalAngle) {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), totalAngle, glm::vec3(0, 1, 0));
        glm::vec4 newPos = rotation * glm::vec4(position, 1.0f);
        return glm::vec3(newPos);
    }
}