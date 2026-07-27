#pragma once

#include <cstddef>
#include <memory>

#include "Utils/Passkey.hpp"

#include "Graphics/API.hpp"
#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics {

class CoreContext;

class BufferAllocator;

class AllocatorContext {
public:
    AllocatorContext(
        Passkey<AllocatorContext>,
        std::unique_ptr<BufferAllocator> bufferAllocator);
    ~AllocatorContext();

    static std::unique_ptr<AllocatorContext> Create(const CoreContext &coreContext);
    void Destroy();

    // Getters
    BufferAllocator &GetBufferAllocator();
    const BufferAllocator &GetBufferAllocator() const;

private:
    std::unique_ptr<BufferAllocator> m_bufferAllocator;
};

}
