#pragma once

#include <volk.h>

#include <vk_mem_alloc.h>

#include "Graphics/Types/BufferTypes.hpp"

namespace Engine::Graphics::Vulkan {

struct BufferCreateInfo {
    VkBufferCreateInfo vkBufferCreateInfo {};
    VmaAllocationCreateInfo vmaAllocationCreateInfo {};
};

VkBufferUsageFlags MapBufferUsage(BufferUsage usage);
BufferCreateInfo MapBufferCreateInfo(
    const Graphics::BufferCreateInfo &info,
    bool isStaged);

}
