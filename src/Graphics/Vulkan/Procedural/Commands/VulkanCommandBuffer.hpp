#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

#include "../Sync/VulkanFence.hpp"
#include "../Sync/VulkanSemaphore.hpp"

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics::Vulkan {

class AllocatorBackend;

struct LogicalDevice;
struct RenderPass;
struct GraphicsPipeline;
struct Framebuffer;
struct CommandPool;

struct CommandBuffer {
    VkCommandBuffer handle = VK_NULL_HANDLE;
};

CommandBuffer AllocateCommandBuffer(
    const LogicalDevice &logicalDevice,
    const CommandPool &commandPool);
void FreeCommandBuffer(
    VkDevice vkDevice,
    CommandBuffer &commandBuffer,
    const CommandPool &commandPool);

void BeginCommandBuffer(CommandBuffer &commandBuffer);
void EndCommandBuffer(CommandBuffer &commandBuffer);

void SubmitCommandBuffer(
    VkQueue vkQueue,
    const CommandBuffer &commandBuffer,
    const std::optional<Semaphore> &waitSemaphore = std::nullopt,
    const std::optional<VkPipelineStageFlags> &waitStage = std::nullopt,
    const std::optional<Semaphore> &signalSemaphore = std::nullopt,
    const std::optional<Fence> &fence = std::nullopt);

void BeginRenderPass(
    CommandBuffer &commandBuffer,
    const RenderPass &renderPass,
    const Framebuffer &framebuffer,
    VkExtent2D extent);
void EndRenderPass(CommandBuffer &commandBuffer);

void BindPipeline(
    CommandBuffer &commandBuffer,
    const GraphicsPipeline &graphicsPipeline,
    VkExtent2D extent);

void CopyBuffer(
    CommandBuffer &commandBuffer,
    const AllocatorBackend &allocator,
    BufferHandle source,
    BufferHandle destination,
    VkDeviceSize size);

void BindVertexBuffer(
    CommandBuffer &commandBuffer,
    const AllocatorBackend &allocator,
    BufferHandle buffer,
    uint32_t firstBinding,
    uint32_t bindingCount,
    VkDeviceSize offset = 0);

void BindIndexBuffer(
    CommandBuffer &commandBuffer,
    const AllocatorBackend &allocator,
    BufferHandle buffer,
    VkDeviceSize offset = 0,
    VkIndexType indexType = VK_INDEX_TYPE_UINT32);

void Draw(
    CommandBuffer &commandBuffer,
    uint32_t vertexCount,
    uint32_t instanceCount,
    uint32_t firstVertex,
    uint32_t firstInstance);

void DrawIndexed(
    CommandBuffer &commandBuffer,
    uint32_t indexCount,
    uint32_t instanceCount,
    uint32_t firstIndex,
    int32_t vertexOffset,
    uint32_t firstInstance);

}
