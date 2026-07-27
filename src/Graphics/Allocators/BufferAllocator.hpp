#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Graphics/API.hpp"
#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics {

class CoreContext;

struct BufferCreateInfo;

class BufferAllocator {
public:
    virtual ~BufferAllocator() = default;

    static std::unique_ptr<BufferAllocator> Create(const CoreContext &coreContext);
    virtual void Destroy() = 0;

    virtual BufferHandle CreateBuffer(
        const BufferCreateInfo &info,
        const void *data = nullptr,
        std::size_t offset = 0) = 0;
    virtual void DestroyBuffer(BufferHandle handle) = 0;
    virtual void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
