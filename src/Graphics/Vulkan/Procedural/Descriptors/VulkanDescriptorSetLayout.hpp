#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct DescriptorSetLayout {
    VkDescriptorSetLayout handle = VK_NULL_HANDLE;
};

}
