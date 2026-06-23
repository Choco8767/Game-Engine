#define VMA_IMPLEMENTATION

#include "VulkanAllocator.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "Graphics/Types/BufferTypes.hpp"
#include "Helpers/VulkanBufferTypes.hpp"

#include "VulkanCoreContext.hpp"

#include "Procedural/Commands/VulkanCommandBuffer.hpp"
#include "Procedural/Core/VulkanInstance.hpp"
#include "Procedural/Core/VulkanLogicalDevice.hpp"
#include "Procedural/Core/VulkanPhysicalDevice.hpp"

namespace Engine::Graphics::Vulkan {

AllocatorBackend::AllocatorBackend(const CoreContextBackend &coreContext)
    : m_coreContext(coreContext)
{
}

AllocatorBackend::~AllocatorBackend()
{
    Destroy();
}

void AllocatorBackend::Init()
{
    VmaVulkanFunctions vmaVulkanFunctions {
        .vkGetInstanceProcAddr = &vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = &vkGetDeviceProcAddr
    };

    VmaAllocatorCreateInfo allocatorInfo = {
        .physicalDevice = m_coreContext.GetPhysicalDevice().handle,
        .device = m_coreContext.GetLogicalDevice().handle,
        .pVulkanFunctions = &vmaVulkanFunctions,
        .instance = m_coreContext.GetInstance().handle,
        .vulkanApiVersion = VK_API_VERSION_1_4
    };

    VkResult vkResult = vmaCreateAllocator(&allocatorInfo, &m_handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Memory Allocator. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Memory Allocator Created Successfully.\n";

    m_commandPool = CreateCommandPool(
        m_coreContext.GetLogicalDevice(),
        m_coreContext.GetPhysicalDevice().queueFamilyIndices.transferFamily.value());
}

void AllocatorBackend::Destroy()
{
    WaitIdle(m_coreContext.GetLogicalDevice());

    for (auto &buffer : m_buffers) {
        if (buffer.handle != VK_NULL_HANDLE && buffer.allocation != nullptr) {
            vmaDestroyBuffer(m_handle, buffer.handle, buffer.allocation);
            buffer.handle = VK_NULL_HANDLE;
            buffer.allocation = nullptr;
        }
    }
    m_buffers.clear();
    m_freeBufferIndices.clear();

    DestroyCommandPool(m_coreContext.GetLogicalDevice().handle, m_commandPool);

    if (m_handle != nullptr) {
        vmaDestroyAllocator(m_handle);
        m_handle = nullptr;
    }
}

void MapMemory(
    VmaAllocator allocator,
    VmaAllocation allocation,
    const void *data,
    std::size_t size)
{
    void *mappedData = nullptr;

    VkResult vkResult = vmaMapMemory(allocator, allocation, &mappedData);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Map VMA Memory. Error Code: {}", static_cast<int>(vkResult)));

    std::memcpy(mappedData, data, size);

    vmaUnmapMemory(allocator, allocation);
}

BufferHandle AllocatorBackend::CreateRawBuffer(
    VkBufferCreateInfo vkBufferCreateInfo,
    VmaAllocationCreateInfo vmaAllocationCreateInfo,
    std::size_t size)
{
    VkBuffer vkBuffer = VK_NULL_HANDLE;
    VmaAllocation vmaAllocation = nullptr;

    VkResult vkResult = vmaCreateBuffer(
        m_handle,
        &vkBufferCreateInfo,
        &vmaAllocationCreateInfo,
        &vkBuffer,
        &vmaAllocation,
        nullptr);

    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Buffer. Error Code: {}", static_cast<int>(vkResult)));

    Buffer buffer {
        .handle = vkBuffer,
        .allocation = vmaAllocation,
        .size = size
    };

    BufferHandle handle { };

    if (!m_freeBufferIndices.empty()) {
        handle.id = static_cast<uint32_t>(m_freeBufferIndices.back());
        m_freeBufferIndices.pop_back();
        m_buffers[handle.id] = buffer;
    } else {
        handle.id = static_cast<uint32_t>(m_buffers.size());
        m_buffers.push_back(buffer);
    }

    return handle;
}

BufferHandle AllocatorBackend::CreateBuffer(const BufferCreateInfo &info, const void *data)
{
    VkBufferUsageFlags vkBufferUsageFlags = MapBufferUsage(info.usage);
    VmaAllocationCreateFlags vmaAllocationFlags = 0;

    bool isStaged = (data != nullptr) && (info.size > 0) && (info.usage == BufferUsage::VERTEX || info.usage == BufferUsage::INDEX);
    if (isStaged)
        vkBufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    switch (info.usage) {
    case BufferUsage::VERTEX:
    case BufferUsage::INDEX:
        if (!isStaged)
            vmaAllocationFlags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        break;

    case BufferUsage::UNIFORM:
        vmaAllocationFlags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        break;

    case BufferUsage::STORAGE:
        vmaAllocationFlags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
        break;
    }

    VkBufferCreateInfo vkBufferCreateInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = info.size,
        .usage = vkBufferUsageFlags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };

    VmaAllocationCreateInfo vmaAllocationCreateInfo {
        .flags = vmaAllocationFlags
    };

    BufferHandle buffer = CreateRawBuffer(vkBufferCreateInfo, vmaAllocationCreateInfo, info.size);
    const auto &rawBuffer = GetBuffer(buffer);

    if (!isStaged) {
        MapMemory(m_handle, rawBuffer.allocation, data, info.size);
        return buffer;
    }

    VkBufferCreateInfo vkStagingBufferCreateInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = info.size,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
    };

    VmaAllocationCreateInfo vmaStagingAllocationCreateInfo {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    BufferHandle stagingBuffer = CreateRawBuffer(vkStagingBufferCreateInfo, vmaStagingAllocationCreateInfo, info.size);
    const auto &rawStagingBuffer = GetBuffer(stagingBuffer);

    MapMemory(m_handle, rawStagingBuffer.allocation, data, info.size);

    auto commandBuffer = AllocateCommandBuffer(m_coreContext.GetLogicalDevice(), m_commandPool);
    BeginCommandBuffer(commandBuffer);

    CopyBuffer(commandBuffer, *this, stagingBuffer, buffer, info.size);

    EndCommandBuffer(commandBuffer);
    SubmitCommandBuffer(m_coreContext.GetLogicalDevice().transferQueue, commandBuffer);

    WaitIdle(m_coreContext.GetLogicalDevice());

    FreeCommandBuffer(m_coreContext.GetLogicalDevice().handle, commandBuffer, m_commandPool);
    DestroyBuffer(stagingBuffer);

    return buffer;
}

void AllocatorBackend::DestroyBuffer(BufferHandle handle)
{
    if (handle.id >= m_buffers.size())
        throw std::runtime_error(std::format("Invalid Buffer Handle: {}", handle.id));

    Buffer &buffer = m_buffers[handle.id];

    if (buffer.handle != VK_NULL_HANDLE && buffer.allocation != nullptr) {
        WaitIdle(m_coreContext.GetLogicalDevice());
        vmaDestroyBuffer(m_handle, buffer.handle, buffer.allocation);

        buffer.handle = VK_NULL_HANDLE;
        buffer.allocation = nullptr;
    }

    m_freeBufferIndices.push_back(handle.id);
}

}
