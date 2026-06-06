#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct ShaderModule {
    VkShaderModule handle = VK_NULL_HANDLE;
};

ShaderModule CreateShaderModule(
    const LogicalDevice &logicalDevice,
    const std::vector<char> &data);
void DestroyShaderModule(VkDevice vkDevice, ShaderModule &shaderModule);

}
