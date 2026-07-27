#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Utils/Passkey.hpp"

#include "Graphics/Allocators/BufferAllocator.hpp"

#include "Graphics/Vulkan/Procedural/Commands/VulkanCommandPool.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;

struct Buffer;

class BufferAllocatorBackend final : public Engine::Graphics::BufferAllocator {
public:
    BufferAllocatorBackend(
        Passkey<BufferAllocatorBackend>,
        const CoreContextBackend &coreContext);
    ~BufferAllocatorBackend() override;

    BufferAllocatorBackend(const BufferAllocatorBackend &other) = delete;
    BufferAllocatorBackend &operator=(const BufferAllocatorBackend &other) = delete;

    BufferAllocatorBackend(BufferAllocatorBackend &&other) noexcept = default;
    BufferAllocatorBackend &operator=(BufferAllocatorBackend &&other) noexcept = default;

    static std::unique_ptr<BufferAllocatorBackend> Create(const CoreContextBackend &coreContext);
    void Destroy() override;

    BufferHandle CreateBuffer(
        const Graphics::BufferCreateInfo &info,
        const void *data = nullptr,
        std::size_t offset = 0) override;
    void DestroyBuffer(BufferHandle handle) override;
    void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) override;

    // Getters
    API GetAPIType() const noexcept override { return API::VULKAN; }

    Buffer &GetBuffer(BufferHandle handle);
    const Buffer &GetBuffer(BufferHandle handle) const;

private:
    const CoreContextBackend &m_coreContext;

    std::vector<Buffer> m_buffers;
    std::vector<std::size_t> m_freeBufferIndices;
};

}
