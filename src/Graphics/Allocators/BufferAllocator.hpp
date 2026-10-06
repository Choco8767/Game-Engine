#pragma once

#include <cstddef>
#include <memory>

#include "Graphics/API.hpp"
#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics {

struct BufferCreateInfo;

class BufferAllocator {
public:
    virtual ~BufferAllocator() = default;

    virtual void Destroy() = 0;

    virtual BufferHandle CreateBuffer(
        const BufferCreateInfo &info,
        const void *data = nullptr,
        std::size_t offset = 0)
        = 0;
    virtual void DestroyBuffer(BufferHandle handle) = 0;
    virtual void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
