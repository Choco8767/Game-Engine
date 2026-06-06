#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "../Rendering/VulkanFramebuffer.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics::Vulkan {

struct Surface;
struct PhysicalDevice;
struct LogicalDevice;
struct RenderPass;
struct Semaphore;

struct Swapchain {
    VkSwapchainKHR handle = VK_NULL_HANDLE;

    VkFormat format = VK_FORMAT_UNDEFINED;
    VkExtent2D extent = { 0, 0 };

    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<Framebuffer> framebuffers;
};

struct AcquireNextSwapchainImageResult {
    uint32_t imageIndex;
    VkResult result;

    bool NeedsRecreation() const
    {
        return result == VK_ERROR_OUT_OF_DATE_KHR;
    }
};

struct PresentSwapchainImageResult {
    VkResult result;

    bool NeedsRecreation() const
    {
        return result == VK_ERROR_OUT_OF_DATE_KHR
            || result == VK_SUBOPTIMAL_KHR;
    }
};

Swapchain CreateSwapchain(
    const Engine::Window::Window &window,
    const Surface &surface,
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice);
void DestroySwapchain(VkDevice vkDevice, Swapchain &swapchain);

void RecreateSwapchain(
    const Engine::Window::Window &window,
    const Surface &surface,
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    Swapchain &swapchain);

void InitSwapchainImageViews(
    const LogicalDevice &logicalDevice,
    Swapchain &swapchain);
void InitSwapchainFramebuffers(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    Swapchain &swapchain);

AcquireNextSwapchainImageResult AcquireNextSwapchainImage(
    const LogicalDevice &logicalDevice,
    const Semaphore &imageAvailable,
    Swapchain &swapchain);

PresentSwapchainImageResult PresentSwapchainImage(
    VkQueue vkPresentQueue,
    const Swapchain &swapchain,
    uint32_t imageIndex,
    const Semaphore &renderFinished);
}
