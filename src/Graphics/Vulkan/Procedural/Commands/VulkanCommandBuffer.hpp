#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct LogicalDevice;
struct RenderPass;
struct GraphicsPipeline;
struct Framebuffer;
struct CommandPool;
struct Semaphore;
struct Fence;

struct CommandBuffer {
    VkCommandBuffer handle = VK_NULL_HANDLE;
};

std::optional<CommandBuffer> AllocateCommandBuffer(
    const LogicalDevice &logicalDevice,
    const CommandPool &commandPool);
void FreeCommandBuffer(
    VkDevice vkDevice,
    CommandBuffer &commandBuffer,
    const CommandPool &commandPool);

void BeginCommandBuffer(CommandBuffer &commandBuffer);
void EndCommandBuffer(CommandBuffer &commandBuffer);

void SubmitCommandBuffer(
    VkQueue vkGraphicsQueue,
    const CommandBuffer &commandBuffer,
    Semaphore imageAvailable,
    Semaphore renderFinished,
    Fence inFlight);

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
