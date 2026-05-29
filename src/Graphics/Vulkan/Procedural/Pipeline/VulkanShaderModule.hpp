#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace Engine {

class Window;

}

namespace Engine::Vulkan {

struct LogicalDevice;

struct ShaderModule {
    VkShaderModule handle = VK_NULL_HANDLE;
};

std::optional<ShaderModule> CreateShaderModule(
    const LogicalDevice &logicalDevice,
    const std::vector<char> &data);
void DestroyShaderModule(VkDevice vkDevice, ShaderModule &shaderModule);

}
