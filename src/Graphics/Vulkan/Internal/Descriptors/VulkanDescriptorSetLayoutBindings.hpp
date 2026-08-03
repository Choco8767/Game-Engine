#pragma once

#include <cstdint>
#include <vector>

#include <volk.h>

namespace Engine::Graphics {

enum class DescriptorType;
enum class ShaderStage : uint32_t;

}

namespace Engine::Graphics::Vulkan {

struct DescriptorSetLayoutBindings {
    std::vector<VkDescriptorSetLayoutBinding> vkBindings;
};

void AddDescriptorLayoutBinding(
    DescriptorSetLayoutBindings &bindings,
    Graphics::DescriptorType type,
    uint32_t binding,
    uint32_t count,
    Graphics::ShaderStage stages);

}
