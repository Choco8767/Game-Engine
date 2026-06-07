#pragma once

#include <vulkan/vulkan.h>

#include "../VMA.hpp"

namespace Engine::Graphics {

enum class BufferUsage;

}

namespace Engine::Graphics::Vulkan {

VkBufferUsageFlags MapBufferUsage(BufferUsage usage);

}
