#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct Instance;

struct DebugMessenger {
    VkDebugUtilsMessengerEXT handle = VK_NULL_HANDLE;
};

std::optional<DebugMessenger> CreateDebugMessenger(const Instance &instance);
void DestroyDebugMessenger(VkInstance vkInstance, DebugMessenger &messenger);

bool CheckValidationLayerSupport(const std::vector<const char *> &validationLayers);

VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();

}
