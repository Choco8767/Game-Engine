#pragma once

#include <vulkan/vulkan.h>

#include "../VMA.hpp"

namespace Engine::Graphics {

enum class BufferUsage;
enum class MemoryAllocationType;

}

namespace Engine::Graphics::Vulkan {

VkBufferUsageFlags MapBufferUsage(BufferUsage usage);
VmaMemoryUsage MapMemoryType(MemoryAllocationType type);

}
