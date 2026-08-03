#include "VulkanShaderModule.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

#include "../Core/VulkanLogicalDevice.hpp"

namespace Engine::Graphics::Vulkan {

ShaderModule CreateShaderModule(
    const LogicalDevice &logicalDevice,
    const std::vector<char> &data)
{
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = data.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(data.data());

    VkShaderModule handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateShaderModule(logicalDevice.handle, &createInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Shader Module. Error Code: {}", static_cast<int>(vkResult)));

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
