#pragma once

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct DescriptorSet {
    VkDescriptorSet handle = VK_NULL_HANDLE;
};

}
