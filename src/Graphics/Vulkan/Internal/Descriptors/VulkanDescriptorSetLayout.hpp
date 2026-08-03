#pragma once

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct DescriptorSetLayout {
    VkDescriptorSetLayout handle = VK_NULL_HANDLE;
};

}
