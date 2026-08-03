#pragma once

#include <vector>

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;

    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

}
