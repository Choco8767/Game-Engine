#include "VulkanCommandBuffer.hpp"

#include <array>
#include <format>
#include <iostream>
#include <stdexcept>

#include "../Commands/VulkanCommandPool.hpp"
#include "../Core/VulkanLogicalDevice.hpp"
#include "../Pipeline/VulkanGraphicsPipeline.hpp"
#include "../Rendering/VulkanFramebuffer.hpp"
#include "../Rendering/VulkanRenderPass.hpp"
#include "Graphics/Vulkan/VulkanAllocator.hpp"

namespace Engine::Graphics::Vulkan {

CommandBuffer AllocateCommandBuffer(
    const LogicalDevice &logicalDevice,
    const CommandPool &commandPool)
{
    VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool.handle,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer handle = VK_NULL_HANDLE;

    VkResult vkResult = vkAllocateCommandBuffers(logicalDevice.handle, &vkCommandBufferAllocateInfo, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Allocate Vulkan Command Buffers. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Command Buffer Allocated Successfully.\n";

    return CommandBuffer {
        .handle = handle
    };
}

void FreeCommandBuffer(
    VkDevice vkDevice,
    CommandBuffer &commandBuffer,
    const CommandPool &commandPool)
{
    if (commandBuffer.handle != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(vkDevice, commandPool.handle, 1, &commandBuffer.handle);
        commandBuffer.handle = VK_NULL_HANDLE;
    }
}

void BeginCommandBuffer(CommandBuffer &commandBuffer)
{
    VkCommandBufferBeginInfo vkCommandBufferBeginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    VkResult vkResult = vkBeginCommandBuffer(commandBuffer.handle, &vkCommandBufferBeginInfo);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Begin Recording Vulkan Command Buffer. Error Code: " << vkResult << "\n";
    }
}

void EndCommandBuffer(CommandBuffer &commandBuffer)
{
    VkResult vkResult = vkEndCommandBuffer(commandBuffer.handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to End Recording Vulkan Command Buffer. Error Code: " << vkResult << "\n";
    }
}

void SubmitCommandBuffer(
    VkQueue vkQueue,
    const CommandBuffer &commandBuffer,
    const std::optional<Semaphore> &waitSemaphore,
    const std::optional<VkPipelineStageFlags> &waitStage,
    const std::optional<Semaphore> &signalSemaphore,
    const std::optional<Fence> &fence)
{
    VkSubmitInfo vkSubmitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer.handle
    };

    if (waitSemaphore.has_value() && waitStage.has_value()) {
        std::array<VkSemaphore, 1> waitSemaphores = { waitSemaphore->handle };
        std::array<VkPipelineStageFlags, 1> waitStages = { waitStage.value() };

        vkSubmitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        vkSubmitInfo.pWaitSemaphores = &waitSemaphore->handle;
        vkSubmitInfo.pWaitDstStageMask = &waitStage.value();
    }

    if (signalSemaphore.has_value()) {
        std::array<VkSemaphore, 1> signalSemaphores = { signalSemaphore->handle };

        vkSubmitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        vkSubmitInfo.pSignalSemaphores = &signalSemaphore->handle;
    }

    VkFence vkFence = fence.has_value() ? fence->handle : VK_NULL_HANDLE;

    VkResult vkResult = vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, vkFence);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Submit Command Buffer to Queue. Error Code: " << vkResult << "\n";
    }
}

void BeginRenderPass(
    CommandBuffer &commandBuffer,
    const RenderPass &renderPass,
    const Framebuffer &framebuffer,
    VkExtent2D extent)
{
    std::vector<VkClearValue> vkClearValues = {
        { { { 0.0f, 0.0f, 0.0f, 1.0f } } }
    };

    VkRenderPassBeginInfo vkRenderPassBeginInfo {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = renderPass.handle,
        .framebuffer = framebuffer.handle,
        .renderArea = {
            .offset = { 0, 0 },
            .extent = extent },
        .clearValueCount = static_cast<uint32_t>(vkClearValues.size()),
        .pClearValues = vkClearValues.data()
    };

    vkCmdBeginRenderPass(commandBuffer.handle, &vkRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void EndRenderPass(CommandBuffer &commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer.handle);
}

void BindPipeline(
    CommandBuffer &commandBuffer,
    const GraphicsPipeline &graphicsPipeline,
    VkExtent2D extent)
{
    vkCmdBindPipeline(commandBuffer.handle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.handle);

    VkViewport vkViewport {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };
    vkCmdSetViewport(commandBuffer.handle, 0, 1, &vkViewport);

    VkRect2D vkScissor {
        .offset = { 0, 0 },
        .extent = extent
    };
    vkCmdSetScissor(commandBuffer.handle, 0, 1, &vkScissor);
}

void CopyBuffer(
    CommandBuffer &commandBuffer,
    const AllocatorBackend &allocator,
    BufferHandle source,
    BufferHandle destination,
    VkDeviceSize size)
{
    VkBufferCopy vkCopyRegion {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size
    };

    Buffer rawSourceBuffer = allocator.GetBuffer(source);
    Buffer rawDestinationBuffer = allocator.GetBuffer(destination);

    vkCmdCopyBuffer(
        commandBuffer.handle,
        rawSourceBuffer.handle,
        rawDestinationBuffer.handle,
        1,
        &vkCopyRegion);
}

void BindVertexBuffer(
    CommandBuffer &commandBuffer,
    const AllocatorBackend &allocator,
    BufferHandle buffer,
    uint32_t firstBinding,
    uint32_t bindingCount,
    VkDeviceSize offset)
{
    Buffer rawBuffer = allocator.GetBuffer(buffer);

    vkCmdBindVertexBuffers(
        commandBuffer.handle,
        firstBinding,
        bindingCount,
        &rawBuffer.handle,
        &offset);
}

void BindIndexBuffer(
    CommandBuffer &commandBuffer,
    const AllocatorBackend &allocator,
    BufferHandle buffer,
    VkDeviceSize offset,
    VkIndexType indexType)
{
    Buffer rawBuffer = allocator.GetBuffer(buffer);

    vkCmdBindIndexBuffer(
        commandBuffer.handle,
        rawBuffer.handle,
        offset,
        indexType);
}

void Draw(
    CommandBuffer &commandBuffer,
    uint32_t vertexCount,
    uint32_t instanceCount,
    uint32_t firstVertex,
    uint32_t firstInstance)
{
    vkCmdDraw(
        commandBuffer.handle,
        vertexCount,
        instanceCount,
        firstVertex,
        firstInstance);
}

void DrawIndexed(
    CommandBuffer &commandBuffer,
    uint32_t indexCount,
    uint32_t instanceCount,
    uint32_t firstIndex,
    int32_t vertexOffset,
    uint32_t firstInstance)
{
    vkCmdDrawIndexed(
        commandBuffer.handle,
        indexCount,
        instanceCount,
        firstIndex,
        vertexOffset,
        firstInstance);
}

}
