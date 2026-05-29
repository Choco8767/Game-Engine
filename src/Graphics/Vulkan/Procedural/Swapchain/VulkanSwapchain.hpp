#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "../Rendering/VulkanFramebuffer.hpp"

namespace Engine {

class Window;

}

namespace Engine::Vulkan {

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

std::optional<Swapchain> CreateSwapchain(
    const Engine::Window &window,
    const Surface &surface,
    const PhysicalDevice &physicalDevice,
    const LogicalDevice &logicalDevice);
void DestroySwapchain(VkDevice vkDevice, Swapchain &swapchain);

void InitSwapchainImageViews(
    const LogicalDevice &logicalDevice,
    Swapchain &swapchain);
void InitSwapchainFramebuffers(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass,
    Swapchain &swapchain);

uint32_t AquireNextSwapchainImage(
    const LogicalDevice &logicalDevice,
    const Semaphore &imageAvailable,
    Swapchain &swapchain);

void PresentSwapchainImage(
    VkQueue vkPresentQueue,
    const Swapchain &swapchain,
    uint32_t imageIndex,
    Semaphore renderFinished);
}
