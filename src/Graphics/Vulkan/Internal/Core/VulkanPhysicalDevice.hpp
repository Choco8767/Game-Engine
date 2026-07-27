#pragma once

#include <vulkan/vulkan.h>

#include "VulkanQueueFamilyIndices.hpp"
#include "VulkanSwapchainSupportDetails.hpp"

namespace Engine::Graphics::Vulkan {

struct Instance;
struct Surface;

struct PhysicalDevice {
    VkPhysicalDevice handle = VK_NULL_HANDLE;

    QueueFamilyIndices queueFamilyIndices;
};

PhysicalDevice CreatePhysicalDevice(
    const Instance &instance,
    const Surface &surface);
void DestroyPhysicalDevice(PhysicalDevice &physicalDevice);

VkPhysicalDeviceProperties GetPhysicalDeviceProperties(const PhysicalDevice &physicalDevice);
VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(const PhysicalDevice &physicalDevice);

SwapchainSupportDetails QuerySwapchainSupport(Surface surface, PhysicalDevice physicalDevice);

}
