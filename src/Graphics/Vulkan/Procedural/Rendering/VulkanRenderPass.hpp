#pragma once

#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct LogicalDevice;
struct Swapchain;

struct RenderPass {
    VkRenderPass handle = VK_NULL_HANDLE;
};

std::optional<RenderPass> CreateRenderPass(const LogicalDevice &logicalDevice, const Swapchain &swapchain);
void DestroyRenderPass(VkDevice vkDevice, RenderPass &renderPass);

}
