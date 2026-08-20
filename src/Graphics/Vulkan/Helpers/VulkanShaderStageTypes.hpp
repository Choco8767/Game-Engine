#pragma once

#include <cstdint>

#include <volk.h>

#include "Graphics/Types/ShaderStageTypes.hpp"

namespace Engine::Graphics::Vulkan {

inline VkShaderStageFlags MapShaderStage(ShaderStage stage)
{
    VkShaderStageFlags vkShaderStageFlags = 0;

    if ((static_cast<std::uint32_t>(stage) & static_cast<std::uint32_t>(Engine::Graphics::ShaderStage::VERTEX)) != 0)
        vkShaderStageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
    if ((static_cast<std::uint32_t>(stage) & static_cast<std::uint32_t>(Engine::Graphics::ShaderStage::FRAGMENT)) != 0)
        vkShaderStageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if ((static_cast<std::uint32_t>(stage) & static_cast<std::uint32_t>(Engine::Graphics::ShaderStage::GEOMETRY)) != 0)
        vkShaderStageFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
    if ((static_cast<std::uint32_t>(stage) & static_cast<std::uint32_t>(Engine::Graphics::ShaderStage::COMPUTE)) != 0)
        vkShaderStageFlags |= VK_SHADER_STAGE_COMPUTE_BIT;

    return vkShaderStageFlags;
}

}
