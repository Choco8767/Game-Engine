#pragma once

#include <cstddef>

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics {

struct BufferCreateInfo;

class Allocator {
public:
    virtual ~Allocator() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    virtual BufferHandle CreateBuffer(const BufferCreateInfo &info, const void *data = nullptr) = 0;
    virtual void DestroyBuffer(BufferHandle handle) = 0;
    virtual void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) = 0;
};

}
