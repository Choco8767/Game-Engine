#pragma once

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanShaderModule.hpp"

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct RenderPass;

struct GraphicsPipeline {
    VkPipeline handle = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
};

GraphicsPipeline CreateGraphicsPipeline(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    std::span<const VkVertexInputBindingDescription> vkVertexBindingDescriptions,
    std::span<const VkVertexInputAttributeDescription> vkVertexAttributeDescriptions);
void DestroyGraphicsPipeline(VkDevice vkDevice, GraphicsPipeline &pipeline);

}
