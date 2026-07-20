#pragma once

#include <cstddef>

#include <vulkan/vulkan.h>

#include "Graphics/Vulkan/VMA.hpp"

namespace Engine::Graphics::Vulkan {

struct Buffer {
    VkBuffer handle = VK_NULL_HANDLE;
    VmaAllocation allocation = nullptr;

    void *mappedData = nullptr;
    std::size_t size = 0;
};

}
