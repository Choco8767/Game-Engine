#pragma once

#include <vector>

#include <volk.h>

namespace Engine::Graphics::Vulkan {

inline const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

}
