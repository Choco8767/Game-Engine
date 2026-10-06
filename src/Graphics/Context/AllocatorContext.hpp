#pragma once

#include <memory>

#include "Graphics/API.hpp"

namespace Engine::Graphics {

class CoreContext;

class BufferAllocator;

class AllocatorContext {
public:
    virtual ~AllocatorContext();

    static std::unique_ptr<AllocatorContext> Create(const CoreContext &coreContext);
    virtual void Destroy() = 0;

    // Getters
    virtual BufferAllocator &GetBufferAllocator() = 0;
    virtual const BufferAllocator &GetBufferAllocator() const = 0;
    virtual API GetAPIType() const noexcept = 0;
};

}
