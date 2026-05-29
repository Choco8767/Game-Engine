#include "VulkanSwapchain.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "Window/Window.hpp"

#include "../Core/VulkanLogicalDevice.hpp"
#include "../Core/VulkanPhysicalDevice.hpp"
#include "../Core/VulkanSurface.hpp"
#include "../Rendering/VulkanRenderPass.hpp"
#include "../Sync/VulkanSemaphore.hpp"

namespace Engine::Vulkan {

VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
{
    for (const auto &format : formats) {
        if (
            format.format == VK_FORMAT_B8G8R8A8_SRGB
            && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    return formats[0];
}

VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &presentModes)
{
    for (const auto &presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return presentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ChooseSwapchainExtent(const Engine::Window &window, const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;

    int width = window.GetWidth();
    int height = window.GetHeight();

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

std::optional<Swapchain> CreateSwapchain(
    const Engine::Window &window,
    const Surface &surface,
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice)
{
    VkSurfaceFormatKHR vkSurfaceFormat = ChooseSwapchainSurfaceFormat(physicalDevice.swapchainSupport.formats);
    VkPresentModeKHR vkPresentMode = ChooseSwapchainPresentMode(physicalDevice.swapchainSupport.presentModes);
    VkExtent2D vkExtent = ChooseSwapchainExtent(window, physicalDevice.swapchainSupport.capabilities);

    uint32_t imageCount = physicalDevice.swapchainSupport.capabilities.minImageCount + 1;

    if (physicalDevice.swapchainSupport.capabilities.maxImageCount > 0
        && imageCount > physicalDevice.swapchainSupport.capabilities.maxImageCount)
        imageCount = physicalDevice.swapchainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR vkSwapchainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface.handle,
        .minImageCount = imageCount,
        .imageFormat = vkSurfaceFormat.format,
        .imageColorSpace = vkSurfaceFormat.colorSpace,
        .imageExtent = vkExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = physicalDevice.swapchainSupport.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = vkPresentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    std::vector<uint32_t> queueFamilyIndices = {
        physicalDevice.queueFamilyIndices.graphicsFamily.value(),
        physicalDevice.queueFamilyIndices.presentFamily.value()
    };

    if (physicalDevice.queueFamilyIndices.graphicsFamily != physicalDevice.queueFamilyIndices.presentFamily) {
        vkSwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        vkSwapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
        vkSwapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        vkSwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkSwapchainCreateInfo.queueFamilyIndexCount = 0;
        vkSwapchainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    VkSwapchainKHR handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateSwapchainKHR(logicalDevice.handle, &vkSwapchainCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Swapchain. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Swapchain Created Successfully.\n";

    std::vector<VkImage> vkImages;
    vkGetSwapchainImagesKHR(logicalDevice.handle, handle, &imageCount, nullptr);
    vkImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice.handle, handle, &imageCount, vkImages.data());

    std::vector<VkImageView> vkImageViews;

    return Swapchain {
        .handle = handle,
        .format = vkSurfaceFormat.format,
        .extent = vkExtent,
        .images = std::move(vkImages),
    };
}

void DestroySwapchain(VkDevice vkDevice, Swapchain &swapchain)
{
    for (auto framebuffer : swapchain.framebuffers) {
        DestroyFramebuffer(vkDevice, framebuffer);
    }

    for (auto imageView : swapchain.imageViews) {
        if (imageView == VK_NULL_HANDLE)
            continue;

        vkDestroyImageView(vkDevice, imageView, nullptr);
        imageView = VK_NULL_HANDLE;
    }

    if (swapchain.handle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(vkDevice, swapchain.handle, nullptr);
        swapchain.handle = VK_NULL_HANDLE;
    }
}

void InitSwapchainImageViews(
    const LogicalDevice &logicalDevice,
    Swapchain &swapchain)
{
    swapchain.imageViews.resize(swapchain.images.size());

    for (size_t i = 0; i < swapchain.images.size(); i++) {
        VkImageViewCreateInfo vkImageViewCreateInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapchain.images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swapchain.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };

        swapchain.imageViews[i] = VK_NULL_HANDLE;

        VkResult vkResult = vkCreateImageView(logicalDevice.handle, &vkImageViewCreateInfo, nullptr, &swapchain.imageViews[i]);
        if (vkResult != VK_SUCCESS) {
            std::cerr << "Failed to Create Vulkan Image Views. Error Code: " << vkResult << "\n";
            return;
        }

        std::cout << "Vulkan Image View Created Successfully for Swapchain Image " << i << ".\n";
    }
}

void InitSwapchainFramebuffers(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    Swapchain &swapchain)
{
    swapchain.framebuffers.resize(swapchain.imageViews.size());

    for (size_t i = 0; i < swapchain.imageViews.size(); i++) {
        std::optional<Framebuffer> framebuffer = CreateFramebuffer(
            logicalDevice,
            renderPass,
            swapchain.extent,
            { swapchain.imageViews[i] });

        if (framebuffer.has_value())
            swapchain.framebuffers[i] = framebuffer.value();
    }
}

uint32_t AquireNextSwapchainImage(
    const LogicalDevice &logicalDevice,
    const Semaphore &imageAvailable,
    Swapchain &swapchain)
{
    uint32_t imageIndex = UINT32_MAX;
    vkAcquireNextImageKHR(logicalDevice.handle, swapchain.handle, UINT64_MAX, imageAvailable.handle, VK_NULL_HANDLE, &imageIndex);

    return imageIndex;
}

void PresentSwapchainImage(
    VkQueue vkPresentQueue,
    const Swapchain &swapchain,
    uint32_t imageIndex,
    Semaphore renderFinished)
{
    std::array<VkSemaphore, 1> signalSemaphores = { renderFinished.handle };
    std::array<VkSwapchainKHR, 1> swapchains = { swapchain.handle };

    VkPresentInfoKHR vkPresentInfo {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
        .pWaitSemaphores = signalSemaphores.data(),
        .swapchainCount = static_cast<uint32_t>(swapchains.size()),
        .pSwapchains = swapchains.data(),
        .pImageIndices = &imageIndex
    };

    vkQueuePresentKHR(vkPresentQueue, &vkPresentInfo);
}

}
