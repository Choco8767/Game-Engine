#include "VulkanInstance.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "VulkanConstants.hpp"

namespace Engine::Graphics::Vulkan {

Instance CreateInstance(const std::vector<const char *> &requiredExtensions)
{
    VkApplicationInfo vkAppInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Game Engine",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_4
    };

    std::vector<const char *> extensions(requiredExtensions.begin(), requiredExtensions.end());
    extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    std::uint32_t extensionPropertyCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, nullptr);

    std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
    if (extensionPropertyCount > 0)
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, extensionProperties.data());

    std::cout << "Available Extensions:\n";
    for (const auto &extensionProperty : extensionProperties)
        std::cout << "\t" << extensionProperty.extensionName << "\n";

    std::vector<const char *> validationLayers;

    VkDebugUtilsMessengerCreateInfoEXT vkDebugMessengerCreateInfo {};
    if (ENABLE_VALIDATION_LAYERS) {
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");

        if (!CheckValidationLayerSupport(validationLayers))
            throw std::runtime_error("Requested Validation Layers, but not Available.");

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        vkDebugMessengerCreateInfo = PopulateDebugMessengerCreateInfo();
    }

    VkInstanceCreateInfo vkInstanceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = ENABLE_VALIDATION_LAYERS ? &vkDebugMessengerCreateInfo : nullptr,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &vkAppInfo,
        .enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size()),
        .ppEnabledLayerNames = validationLayers.data(),
        .enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };

    VkInstance handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Instance. Error Code: {}", static_cast<int>(vkResult)));

    volkLoadInstance(handle);

    std::cout << "Vulkan Instance Created Successfully.\n";

    Instance instance {
        .handle = handle
    };

    std::optional<DebugMessenger> debugMessenger = std::nullopt;
    if (ENABLE_VALIDATION_LAYERS)
        debugMessenger = CreateDebugMessenger(instance);

    return Instance {
        .handle = handle,
        .debugMessenger = debugMessenger
    };
}

void DestroyInstance(Instance &instance)
{
    if (instance.debugMessenger.has_value())
        DestroyDebugMessenger(instance.handle, instance.debugMessenger.value());

    if (instance.handle != VK_NULL_HANDLE) {
        vkDestroyInstance(instance.handle, nullptr);
        instance.handle = VK_NULL_HANDLE;
    }
}

}
