#include "VulkanPhysicalDevice.hpp"

#include <iostream>
#include <set>

#include "VulkanConfig.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"

namespace Engine::Vulkan {

VkPhysicalDeviceProperties GetDeviceProperties(VkPhysicalDevice vkPhysicalDevice)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);

    return properties;
}

VkPhysicalDeviceFeatures GetDeviceFeatures(VkPhysicalDevice vkPhysicalDevice)
{
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &deviceFeatures);

    return deviceFeatures;
}

int RateDeviceSuitability(
    VkPhysicalDevice vkPhysicalDevice,
    VkPhysicalDeviceProperties properties,
    VkPhysicalDeviceFeatures features,
    QueueFamilyIndices indices)
{
    if (
        !features.geometryShader
        || !indices.IsComplete())
        return 0;

    int suitability = 0;

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        suitability += 1000;

    suitability += properties.limits.maxImageDimension2D;

    return suitability;
}

bool CheckRequiredDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice, const std::vector<const char *> deviceExtensions)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> pxtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, pxtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : pxtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR vkSurface, VkPhysicalDevice vkPhysicalDevice)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        VkBool32 hasPresentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, vkSurface, &hasPresentSupport);

        if (hasPresentSupport)
            indices.presentFamily = i;

        if (indices.IsComplete())
            break;

        i++;
    }

    return indices;
}

SwapchainSupportDetails QuerySwapchainSupport(VkSurfaceKHR vkSurface, VkPhysicalDevice vkPhysicalDevice)
{
    SwapchainSupportDetails details;

    // Capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        vkPhysicalDevice,
        vkSurface,
        &details.capabilities);

    // Formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        vkPhysicalDevice,
        vkSurface,
        &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            vkPhysicalDevice,
            vkSurface,
            &formatCount,
            details.formats.data());
    }

    // Present Modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        vkPhysicalDevice,
        vkSurface,
        &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            vkPhysicalDevice,
            vkSurface,
            &presentModeCount,
            details.presentModes.data());
    }

    return details;
}

QueueFamilyIndices FindQueueFamilies(Surface surface, PhysicalDevice physicalDevice)
{
    return FindQueueFamilies(surface.handle, physicalDevice.handle);
}

SwapchainSupportDetails QuerySwapchainSupport(Surface surface, PhysicalDevice physicalDevice)
{
    return QuerySwapchainSupport(surface.handle, physicalDevice.handle);
}

std::optional<PhysicalDevice> CreatePhysicalDevice(
    const Instance &instance,
    const Surface &surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.handle, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cerr << "Failed to Find a GPU with Vulkan Support.\n";
        return std::nullopt;
    }

    std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance.handle, &deviceCount, vkPhysicalDevices.data());

    VkPhysicalDevice handle = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties properties = { };
    VkPhysicalDeviceFeatures features = { };
    QueueFamilyIndices indices { };
    SwapchainSupportDetails swapchainSupport { };

    int bestSuitability = -1;

    for (const auto &vkPhysicalDevice : vkPhysicalDevices) {
        if (!CheckRequiredDeviceExtensionSupport(vkPhysicalDevice, DEVICE_EXTENSIONS))
            continue;

        bool supportsAdequateSwapchain = false;

        SwapchainSupportDetails currentSwapchainSupport = QuerySwapchainSupport(surface.handle, vkPhysicalDevice);
        supportsAdequateSwapchain = !currentSwapchainSupport.formats.empty()
            && !currentSwapchainSupport.presentModes.empty();

        if (!supportsAdequateSwapchain)
            continue;

        VkPhysicalDeviceProperties currentProperties = GetDeviceProperties(vkPhysicalDevice);
        VkPhysicalDeviceFeatures currentFeatures = GetDeviceFeatures(vkPhysicalDevice);
        QueueFamilyIndices currentIndices = FindQueueFamilies(surface.handle, vkPhysicalDevice);

        int suitability = RateDeviceSuitability(vkPhysicalDevice, currentProperties, currentFeatures, currentIndices);

        if (suitability > bestSuitability) {
            handle = vkPhysicalDevice;
            bestSuitability = suitability;

            properties = currentProperties;
            features = currentFeatures;
            indices = currentIndices;
            swapchainSupport = currentSwapchainSupport;
        }
    }

    if (handle == VK_NULL_HANDLE) {
        std::cerr << "Failed to Find a Suitable GPU.\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Physical Device Selected Successfully.\n";
    std::cout << "\tSelected GPU: " << properties.deviceName << "\n";

    return PhysicalDevice {
        .handle = handle,
        .deviceProperties = properties,
        .deviceFeatures = features,
        .queueFamilyIndices = indices,
        .swapchainSupport = std::move(swapchainSupport)
    };
}

void DestroyPhysicalDevice(PhysicalDevice &physicalDevice)
{
    physicalDevice.handle = VK_NULL_HANDLE;
    physicalDevice.deviceProperties = { };
    physicalDevice.deviceFeatures = { };
    physicalDevice.queueFamilyIndices = { };
    physicalDevice.swapchainSupport = { };
}

}
