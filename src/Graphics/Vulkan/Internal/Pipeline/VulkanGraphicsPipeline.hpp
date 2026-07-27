#pragma once

#include <span>

#include <vulkan/vulkan.h>

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct RenderPass;

class DescriptorSetLayoutRegistry;

struct GraphicsPipeline {
    VkPipeline handle = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
};

GraphicsPipeline CreateGraphicsPipeline(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    const DescriptorSetLayoutRegistry &descriptorSetLayoutRegistry,
    std::span<const VkVertexInputBindingDescription> vkVertexBindingDescriptions,
    std::span<const VkVertexInputAttributeDescription> vkVertexAttributeDescriptions,
    std::span<const DescriptorSetLayoutHandle> descriptorSetLayouts);
void DestroyGraphicsPipeline(VkDevice vkDevice, GraphicsPipeline &pipeline);

}
