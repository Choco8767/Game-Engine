#pragma once

#include <vulkan/vulkan.h>

#include "Graphics/Types/BufferTypes.hpp"

namespace Engine::Graphics::Vulkan {

VkBufferUsageFlags MapBufferUsage(BufferUsage usage)
{
    switch (usage) {
    case Engine::Graphics::BufferUsage::VERTEX:
        return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case Engine::Graphics::BufferUsage::INDEX:
        return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    case Engine::Graphics::BufferUsage::UNIFORM:
        return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case Engine::Graphics::BufferUsage::STORAGE:
        return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    default:
        return 0;
    }
}

}
