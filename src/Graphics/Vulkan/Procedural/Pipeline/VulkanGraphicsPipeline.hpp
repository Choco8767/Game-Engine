#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include "VulkanShaderModule.hpp"

namespace Engine::Vulkan {

struct LogicalDevice;
struct RenderPass;

struct GraphicsPipeline {
    VkPipeline handle = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;

    std::optional<ShaderModule> vertexShaderModule = std::nullopt;
    std::optional<ShaderModule> fragmentShaderModule = std::nullopt;
};

std::optional<GraphicsPipeline> CreateGraphicsPipeline(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass);
void DestroyGraphicsPipeline(VkDevice vkDevice, GraphicsPipeline &pipeline);

}
