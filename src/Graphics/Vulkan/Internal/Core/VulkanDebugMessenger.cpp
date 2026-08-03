#include "VulkanInstance.hpp"

#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>

#include "VulkanConstants.hpp"

namespace Engine::Graphics::Vulkan {

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    std::cerr << "Validation Layer: " << pCallbackData->pMessage << "\n";

    return VK_FALSE;
}

DebugMessenger CreateDebugMessenger(const Instance &instance)
{
    VkDebugUtilsMessengerCreateInfoEXT vkDebugMessengerCreateInfo {};
    if (ENABLE_VALIDATION_LAYERS) {
        vkDebugMessengerCreateInfo = PopulateDebugMessengerCreateInfo();
    }

    VkDebugUtilsMessengerEXT handle = VK_NULL_HANDLE;

    VkResult vkResult = CreateDebugUtilsMessengerEXT(instance.handle, &vkDebugMessengerCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Debug Messenger. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Debug Messenger Created Successfully.\n";

    return DebugMessenger {
        .handle = handle
    };
}

void DestroyDebugMessenger(VkInstance vkInstance, DebugMessenger &messenger)
{
    if (messenger.handle != VK_NULL_HANDLE && ENABLE_VALIDATION_LAYERS) {
        DestroyDebugUtilsMessengerEXT(vkInstance, messenger.handle, nullptr);
        messenger.handle = VK_NULL_HANDLE;
    }
}

bool CheckValidationLayerSupport(const std::vector<const char *> &validationLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            return false;
    }

    return true;
}

VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = DebugCallback,
        .pUserData = nullptr
    };

    return createInfo;
}

}
