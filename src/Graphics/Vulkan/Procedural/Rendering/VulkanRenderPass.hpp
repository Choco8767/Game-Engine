#pragma once

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct Swapchain;

struct RenderPass {
    VkRenderPass handle = VK_NULL_HANDLE;
};

RenderPass CreateRenderPass(const LogicalDevice &logicalDevice, const Swapchain &swapchain);
void DestroyRenderPass(VkDevice vkDevice, RenderPass &renderPass);

}
