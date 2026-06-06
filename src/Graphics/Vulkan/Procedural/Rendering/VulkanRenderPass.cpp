#include "VulkanRenderPass.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "../Core/VulkanLogicalDevice.hpp"
#include "../Swapchain/VulkanSwapchain.hpp"

namespace Engine::Graphics::Vulkan {

RenderPass CreateRenderPass(const LogicalDevice &logicalDevice, const Swapchain &swapchain)
{
    std::vector<VkAttachmentDescription> vkColorAttachments = {
        {
            // Color
            .format = swapchain.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        }
    };

    VkAttachmentReference vkColorAttachmentRef {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    std::vector<VkSubpassDescription> vkSubpassDescriptions = {
        {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &vkColorAttachmentRef,
        }
    };

    std::vector<VkSubpassDependency> vkSubpassDependencies = {
        {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        }
    };

    VkRenderPassCreateInfo vkRenderPassCreateInfo {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(vkColorAttachments.size()),
        .pAttachments = vkColorAttachments.data(),
        .subpassCount = static_cast<uint32_t>(vkSubpassDescriptions.size()),
        .pSubpasses = vkSubpassDescriptions.data(),
        .dependencyCount = static_cast<uint32_t>(vkSubpassDependencies.size()),
        .pDependencies = vkSubpassDependencies.data()
    };

    VkRenderPass handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateRenderPass(logicalDevice.handle, &vkRenderPassCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Render Pass. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Render Pass Created Successfully.\n";

    return RenderPass {
        .handle = handle
    };
}

void DestroyRenderPass(VkDevice vkDevice, RenderPass &renderPass)
{
    if (renderPass.handle != VK_NULL_HANDLE) {
        vkDestroyRenderPass(vkDevice, renderPass.handle, nullptr);
        renderPass.handle = VK_NULL_HANDLE;
    }
}
}
