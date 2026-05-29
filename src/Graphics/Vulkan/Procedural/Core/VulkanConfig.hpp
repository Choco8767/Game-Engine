#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

inline const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

}
