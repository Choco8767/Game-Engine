#pragma once

#include <cstddef>
#include <cstdint>

namespace Engine::Graphics {

enum class BufferUsage : std::uint8_t {
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
