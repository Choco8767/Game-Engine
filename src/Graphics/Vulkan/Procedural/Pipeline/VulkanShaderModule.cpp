#include "VulkanShaderModule.hpp"

#include <iostream>

#include "../Core/VulkanLogicalDevice.hpp"

namespace Engine::Vulkan {

std::optional<ShaderModule> CreateShaderModule(
    const LogicalDevice &logicalDevice,
    const std::vector<char> &data)
{
    VkShaderModuleCreateInfo createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = data.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(data.data());

    VkShaderModule handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateShaderModule(logicalDevice.handle, &createInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Shader Module. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Shader Module Created Successfully.\n";

    return ShaderModule {
        .handle = handle
    };
}

void DestroyShaderModule(VkDevice vkDevice, ShaderModule &shaderModule)
{
    if (shaderModule.handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vkDevice, shaderModule.handle, nullptr);
        shaderModule.handle = VK_NULL_HANDLE;
    }
}

}
