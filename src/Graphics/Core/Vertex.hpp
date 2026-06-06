#pragma once

#include <glm/glm.hpp>

namespace Engine::Graphics {

struct Vertex {
    glm::vec3 position { 0.0f, 0.0f, 0.0f };
    glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
};

}
