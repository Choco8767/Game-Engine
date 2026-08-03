#pragma once

#include <vector>

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct Instance;

struct DebugMessenger {
    VkDebugUtilsMessengerEXT handle = VK_NULL_HANDLE;
};

DebugMessenger CreateDebugMessenger(const Instance &instance);
void DestroyDebugMessenger(VkInstance vkInstance, DebugMessenger &messenger);

bool CheckValidationLayerSupport(const std::vector<const char *> &validationLayers);

VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();

}
