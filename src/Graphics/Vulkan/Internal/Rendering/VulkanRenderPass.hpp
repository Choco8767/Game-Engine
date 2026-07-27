#pragma once

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct Surface;
struct LogicalDevice;

struct RenderPass {
    VkRenderPass handle = VK_NULL_HANDLE;
};

RenderPass CreateRenderPass(const LogicalDevice &logicalDevice, const Surface &surface);
void DestroyRenderPass(VkDevice vkDevice, RenderPass &renderPass);

}
