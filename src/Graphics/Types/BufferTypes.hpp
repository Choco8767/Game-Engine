#pragma once

#include <cstddef>

namespace Engine::Graphics {

enum class BufferUsage {
    VERTEX,
    INDEX,
    UNIFORM,
    STORAGE
};

struct BufferCreateInfo {
    std::size_t size = 0;
    BufferUsage usage;
};

}
