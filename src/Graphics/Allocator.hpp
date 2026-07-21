#pragma once

#include <cstddef>
#include <memory>

#include "API.hpp"

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics {

class CoreContext;

struct BufferCreateInfo;

class Allocator {
public:
    virtual ~Allocator() = default;

    static std::unique_ptr<Allocator> Create(const CoreContext &coreContext);
    virtual void Destroy() = 0;

    virtual BufferHandle CreateBuffer(const BufferCreateInfo &info, const void *data = nullptr) = 0;
    virtual void DestroyBuffer(BufferHandle handle) = 0;
    virtual void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
