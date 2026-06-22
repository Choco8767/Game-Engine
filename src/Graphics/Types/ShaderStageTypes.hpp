
#pragma once

#include <cstdint>

namespace Engine::Graphics {

enum class ShaderStage : uint32_t {
    NONE = 0,
    VERTEX = 1 << 0,
    FRAGMENT = 1 << 1,
    GEOMETRY = 1 << 2,
    COMPUTE = 1 << 3,

    ALL = VERTEX | FRAGMENT | GEOMETRY | COMPUTE
};

}
