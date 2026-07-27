#pragma once

#include <cstddef>

#include <vulkan/vulkan.h>

#include "../Memory/VulkanMemoryAllocation.hpp"

namespace Engine::Graphics::Vulkan {

struct Buffer {
    VkBuffer handle = VK_NULL_HANDLE;

    MemoryAllocation allocation { };
};

}
