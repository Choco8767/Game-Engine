#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include "VulkanQueueFamilyIndices.hpp"
#include "VulkanSwapchainSupportDetails.hpp"

namespace Engine::Vulkan {

struct Instance;
struct Surface;

struct PhysicalDevice {
    VkPhysicalDevice handle = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    QueueFamilyIndices queueFamilyIndices;
    SwapchainSupportDetails swapchainSupport;
};

std::optional<PhysicalDevice> CreatePhysicalDevice(
    const Instance &instance,
    const Surface &surface);
void DestroyPhysicalDevice(PhysicalDevice &physicalDevice);

QueueFamilyIndices FindQueueFamilies(Surface surface, PhysicalDevice physicalDevice);
SwapchainSupportDetails QuerySwapchainSupport(Surface surface, PhysicalDevice physicalDevice);

}
