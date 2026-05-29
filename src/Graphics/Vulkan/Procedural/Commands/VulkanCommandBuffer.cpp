#include "VulkanCommandBuffer.hpp"

#include <array>
#include <iostream>

#include "../Core/VulkanLogicalDevice.hpp"
#include "../Pipeline/VulkanGraphicsPipeline.hpp"
#include "../Rendering/VulkanFramebuffer.hpp"
#include "../Rendering/VulkanRenderPass.hpp"
#include "../Sync/VulkanFence.hpp"
#include "../Sync/VulkanSemaphore.hpp"
#include "VulkanCommandPool.hpp"

namespace Engine::Vulkan {

std::optional<CommandBuffer> AllocateCommandBuffer(
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
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Allocate Vulkan Command Buffers. Error Code: " << vkResult << "\n";
    }

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
    VkQueue vkGraphicsQueue,
    const CommandBuffer &commandBuffer,
    Semaphore imageAvailable,
    Semaphore renderFinished,
    Fence inFlight)
{
    std::array<VkSemaphore, 1> signalSemaphores = { renderFinished.handle };
    std::array<VkSemaphore, 1> waitSemaphores = { imageAvailable.handle };
    std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo vkSubmitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()),
        .pWaitSemaphores = waitSemaphores.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer.handle,
        .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data()
    };

    VkResult vkResult = vkQueueSubmit(vkGraphicsQueue, 1, &vkSubmitInfo, inFlight.handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Submit Command Buffer to Graphics Queue. Error Code: " << vkResult << "\n";
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
