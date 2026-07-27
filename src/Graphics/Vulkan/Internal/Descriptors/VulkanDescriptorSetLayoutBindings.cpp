#include "VulkanDescriptorSetLayoutBindings.hpp"

#include "Graphics/Types/DescriptorTypes.hpp"
#include "Graphics/Types/ShaderStageTypes.hpp"
#include "Graphics/Vulkan/Helpers/VulkanDescriptorTypes.hpp"
#include "Graphics/Vulkan/Helpers/VulkanShaderStageTypes.hpp"

namespace Engine::Graphics::Vulkan {

void AddDescriptorLayoutBinding(
    DescriptorSetLayoutBindings &bindings,
    Graphics::DescriptorType type,
    uint32_t binding,
    uint32_t count,
    Graphics::ShaderStage stage)
{
    VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding {
        .binding = binding,
        .descriptorType = MapDescriptorType(type),
        .descriptorCount = count,
        .stageFlags = MapShaderStage(stage)
    };

    bindings.vkBindings.push_back(vkDescriptorSetLayoutBinding);
}

}
