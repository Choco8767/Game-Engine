#include "VulkanBufferAllocator.hpp"

#include <cstdint>
#include <format>
#include <stdexcept>
#include <utility>

#include "Graphics/Types/BufferTypes.hpp"

#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"

#include "Graphics/Vulkan/Helpers/VulkanBufferTypes.hpp"

#include "Graphics/Vulkan/Internal/Commands/VulkanCommandBuffer.hpp"
#include "Graphics/Vulkan/Internal/Core/VulkanLogicalDevice.hpp"
#include "Graphics/Vulkan/Internal/Memory/VulkanMemoryAllocation.hpp"
#include "Graphics/Vulkan/Internal/Memory/VulkanMemoryAllocator.hpp"
#include "Graphics/Vulkan/Internal/Resources/VulkanBuffer.hpp"

namespace Engine::Graphics::Vulkan {

BufferAllocatorBackend::BufferAllocatorBackend(
    Passkey<BufferAllocatorBackend>,
    const CoreContextBackend &coreContext)
    : m_coreContext(coreContext)
{
}

BufferAllocatorBackend::~BufferAllocatorBackend()
{
    Destroy();
}

std::unique_ptr<BufferAllocatorBackend> BufferAllocatorBackend::Create(const CoreContextBackend &coreContext)
{
    return std::make_unique<BufferAllocatorBackend>(
        Passkey<BufferAllocatorBackend> {},
        coreContext);
}

void BufferAllocatorBackend::Destroy()
{
    for (auto &buffer : m_buffers) {
        Vulkan::DestroyBuffer(m_coreContext.get().GetMemoryAllocator(), buffer);
    }

    m_buffers.clear();
    m_freeBufferIndices.clear();
}

BufferHandle BufferAllocatorBackend::CreateBuffer(const Graphics::BufferCreateInfo &info, const void *data, std::size_t offset)
{
    bool isStaged = (data != nullptr)
        && (info.size > 0)
        && (info.usage == BufferUsage::VERTEX || info.usage == BufferUsage::INDEX);

    BufferCreateInfo bufferCreateInfo = MapBufferCreateInfo(info, isStaged);
    Buffer buffer = Vulkan::CreateBuffer(m_coreContext.get().GetMemoryAllocator(), bufferCreateInfo.vkBufferCreateInfo, bufferCreateInfo.vmaAllocationCreateInfo, offset);

    if (isStaged) {
        BufferCreateInfo stagingBufferCreateInfo {
            .vkBufferCreateInfo {
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .size = info.size,
                .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            },
            .vmaAllocationCreateInfo {
                .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                .usage = VMA_MEMORY_USAGE_AUTO,
            }
        };

        Buffer stagingBuffer = Vulkan::CreateBuffer(m_coreContext.get().GetMemoryAllocator(), stagingBufferCreateInfo.vkBufferCreateInfo, stagingBufferCreateInfo.vmaAllocationCreateInfo, offset);
        MapMemory(m_coreContext.get().GetMemoryAllocator(), stagingBuffer.allocation, data, info.size, offset);

        auto commandBuffer = AllocateCommandBuffer(m_coreContext.get().GetLogicalDevice(), m_coreContext.get().GetMemoryAllocator().commandPool);

        BeginCommandBuffer(commandBuffer);
        CopyBuffer(commandBuffer, stagingBuffer, buffer, info.size);
        EndCommandBuffer(commandBuffer);
        SubmitCommandBuffer(m_coreContext.get().GetLogicalDevice().transferQueue, commandBuffer);

        WaitIdle(m_coreContext.get().GetLogicalDevice());
        FreeCommandBuffer(m_coreContext.get().GetLogicalDevice().handle, commandBuffer, m_coreContext.get().GetMemoryAllocator().commandPool);

        Vulkan::DestroyBuffer(m_coreContext.get().GetMemoryAllocator(), stagingBuffer);
    } else if (data != nullptr) {
        MapMemory(m_coreContext.get().GetMemoryAllocator(), buffer.allocation, data, info.size, offset);
    }

    BufferHandle handle {};
    if (!m_freeBufferIndices.empty()) {
        handle.id = static_cast<std::uint32_t>(m_freeBufferIndices.back());
        m_freeBufferIndices.pop_back();
        m_buffers[handle.id] = buffer;
    } else {
        handle.id = static_cast<std::uint32_t>(m_buffers.size());
        m_buffers.push_back(buffer);
    }

    return handle;
}

void BufferAllocatorBackend::DestroyBuffer(BufferHandle handle)
{
    Buffer &buffer = GetBuffer(handle);

    if (buffer.handle == VK_NULL_HANDLE)
        return;

    WaitIdle(m_coreContext.get().GetLogicalDevice());

    Vulkan::DestroyBuffer(m_coreContext.get().GetMemoryAllocator(), buffer);

    m_freeBufferIndices.push_back(handle.id);
}

void BufferAllocatorBackend::UpdateBuffer(BufferHandle handle, const void *data, std::size_t size, std::size_t offset)
{
    const Buffer &buffer = GetBuffer(handle);

    if (buffer.allocation.mappedData == nullptr)
        throw std::runtime_error(std::format("Attempted to Update a Non-Host-Mapped Buffer. ID: {}", handle.id));

    if (offset + size > buffer.allocation.size)
        throw std::runtime_error(std::format("Buffer Update Out of Bounds. Size: {}, Offset: {}, Buffer Capacity: {}", size, offset, buffer.allocation.size));

    MapMemory(m_coreContext.get().GetMemoryAllocator(), buffer.allocation, data, size, offset);
}

// Getters
Buffer &BufferAllocatorBackend::GetBuffer(BufferHandle handle)
{
    return const_cast<Buffer &>(std::as_const(*this).GetBuffer(handle));
}

const Buffer &BufferAllocatorBackend::GetBuffer(BufferHandle handle) const
{
    if (handle.id >= m_buffers.size())
        throw std::runtime_error(std::format("Invalid Buffer Handle ID: {}", handle.id));

    if (m_buffers[handle.id].handle == VK_NULL_HANDLE)
        throw std::runtime_error(std::format("Stale Buffer Handle ID: {}", handle.id));

    return m_buffers[handle.id];
}

}
