#include "VulkanFramebuffer.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>

#include "../Core/VulkanLogicalDevice.hpp"
#include "VulkanRenderPass.hpp"

namespace Engine::Graphics::Vulkan {

Framebuffer CreateFramebuffer(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    const VkExtent2D &extent,
    const std::vector<VkImageView> &imageViews)
{
    VkFramebufferCreateInfo vkFramebufferCreateInfo {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = renderPass.handle,
        .attachmentCount = static_cast<std::uint32_t>(imageViews.size()),
        .pAttachments = imageViews.data(),
        .width = extent.width,
        .height = extent.height,
        .layers = 1
    };

    VkFramebuffer handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateFramebuffer(logicalDevice.handle, &vkFramebufferCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Framebuffer. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Framebuffer Created Successfully.\n";

    return Framebuffer {
        .handle = handle
    };
}

void DestroyFramebuffer(VkDevice vkDevice, Framebuffer &framebuffer)
{
    if (framebuffer.handle != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(vkDevice, framebuffer.handle, nullptr);
        framebuffer.handle = VK_NULL_HANDLE;
    }
}

}
