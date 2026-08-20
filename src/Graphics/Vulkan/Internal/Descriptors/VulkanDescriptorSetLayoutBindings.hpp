#pragma once

#include <cstdint>
#include <vector>

#include <volk.h>

#include "Graphics/Types/DescriptorTypes.hpp"
#include "Graphics/Types/ShaderStageTypes.hpp"

namespace Engine::Graphics::Vulkan {

struct DescriptorSetLayoutBindings {
    std::vector<VkDescriptorSetLayoutBinding> vkBindings;
};

void AddDescriptorLayoutBinding(
    DescriptorSetLayoutBindings &bindings,
    Graphics::DescriptorType type,
    std::uint32_t binding,
    std::uint32_t count,
    Graphics::ShaderStage stages);

}
