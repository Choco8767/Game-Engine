#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct LogicalDevice;
struct RenderPass;

struct Framebuffer {
    VkFramebuffer handle = VK_NULL_HANDLE;
};

std::optional<Framebuffer> CreateFramebuffer(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    const VkExtent2D &extent,
    const std::vector<VkImageView> &imageViews);
void DestroyFramebuffer(VkDevice vkDevice, Framebuffer &framebuffer);

}
