#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Utils/Passkey.hpp"

#include "../Allocator.hpp"

#include "VMA.hpp"

#include "Procedural/Commands/VulkanCommandPool.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;

struct Instance;
struct PhysicalDevice;
struct LogicalDevice;

struct Buffer;

class AllocatorBackend final : public Engine::Graphics::Allocator {
public:
    AllocatorBackend(
        Passkey<AllocatorBackend>,
        const CoreContextBackend &coreContext,
        VmaAllocator handle,
        CommandPool commandPool);
    ~AllocatorBackend() override;

    AllocatorBackend(const AllocatorBackend &other) = delete;
    AllocatorBackend &operator=(const AllocatorBackend &other) = delete;

    AllocatorBackend(AllocatorBackend &&other) noexcept = default;
    AllocatorBackend &operator=(AllocatorBackend &&other) noexcept = default;

    static std::unique_ptr<AllocatorBackend> Create(const CoreContextBackend &coreContext);
    void Destroy() override;

    BufferHandle CreateBuffer(const BufferCreateInfo &info, const void *data = nullptr) override;
    void DestroyBuffer(BufferHandle handle) override;
    void UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset) override;

    // Getters
    API GetAPIType() const noexcept override { return API::VULKAN; }

    const Buffer &GetBuffer(BufferHandle handle) const;

private:
    BufferHandle CreateRawBuffer(
        VkBufferCreateInfo vkBufferCreateInfo,
        VmaAllocationCreateInfo vmaAllocationCreateInfo,
        std::size_t size);

    const CoreContextBackend &m_coreContext;

    CommandPool m_commandPool;

    VmaAllocator m_handle = nullptr;

    std::vector<Buffer> m_buffers;
    std::vector<std::size_t> m_freeBufferIndices;
};

}
