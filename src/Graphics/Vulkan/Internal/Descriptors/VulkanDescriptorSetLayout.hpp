#pragma once

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct DescriptorSetLayout {
    VkDescriptorSetLayout handle = VK_NULL_HANDLE;
};

}
