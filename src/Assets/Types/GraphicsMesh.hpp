#pragma once

#include <cstddef>

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Assets {

struct GraphicsMesh {
    BufferHandle vertexBuffer {};
    BufferHandle indexBuffer {};
    std::size_t vertexCount = 0;
    std::size_t indexCount = 0;
};

}
