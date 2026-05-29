#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;

    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

}
