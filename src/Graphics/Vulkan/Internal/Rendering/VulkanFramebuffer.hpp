#pragma once

#include <vector>

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct RenderPass;

struct Framebuffer {
    VkFramebuffer handle = VK_NULL_HANDLE;
};

Framebuffer CreateFramebuffer(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    const VkExtent2D &extent,
    const std::vector<VkImageView> &imageViews);
void DestroyFramebuffer(VkDevice vkDevice, Framebuffer &framebuffer);

}
