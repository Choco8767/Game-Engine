#pragma once

#include <vulkan/vulkan.h>

#include "Graphics/Types/DescriptorTypes.hpp"

namespace Engine::Graphics::Vulkan {

VkDescriptorType MapDescriptorType(DescriptorType type)
{
    switch (type) {
    case DescriptorType::SAMPLER:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case DescriptorType::COMBINED_IMAGE_SAMPLER:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case DescriptorType::SAMPLED_IMAGE:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case DescriptorType::STORAGE_IMAGE:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case DescriptorType::UNIFORM_TEXEL_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    case DescriptorType::STORAGE_TEXEL_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    case DescriptorType::UNIFORM_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::STORAGE_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case DescriptorType::UNIFORM_BUFFER_DYNAMIC:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    case DescriptorType::STORAGE_BUFFER_DYNAMIC:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    case DescriptorType::INPUT_ATTACHMENT:
        return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    default:
        return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

}
