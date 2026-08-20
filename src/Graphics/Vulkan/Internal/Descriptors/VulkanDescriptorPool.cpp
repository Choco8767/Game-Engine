#include "VulkanDescriptorPool.hpp"

#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../Core/VulkanLogicalDevice.hpp"

#include "VulkanDescriptorSet.hpp"
#include "VulkanDescriptorSetLayout.hpp"
#include "VulkanDescriptorSetLayoutRegistry.hpp"

namespace Engine::Graphics::Vulkan {

DescriptorPool CreateDescriptorPool(const LogicalDevice &logicalDevice, const DescriptorPoolCreateInfo &info)
{
    std::vector<VkDescriptorPoolSize> vkDescriptorPoolSizes = {
        {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = info.uniformBufferCount,
        },
        {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = info.imageSamplerCount,
        }
    };

    VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = info.maxSets,
        .poolSizeCount = static_cast<std::uint32_t>(vkDescriptorPoolSizes.size()),
        .pPoolSizes = vkDescriptorPoolSizes.data()
    };

    VkDescriptorPool handle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateDescriptorPool(logicalDevice.handle, &vkDescriptorPoolCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Descriptor Pool. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Descriptor Pool Created Successfully.\n";

    return DescriptorPool {
        .handle = handle
    };
}

void DestroyDescriptorPool(VkDevice vkDevice, DescriptorPool &descriptorPool)
{
    if (descriptorPool.handle != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(vkDevice, descriptorPool.handle, nullptr);
        descriptorPool.handle = VK_NULL_HANDLE;
    }
}

std::vector<DescriptorSet> AllocateDescriptorSets(
    const LogicalDevice &logicalDevice,
    const DescriptorSetLayoutRegistry &descriptorSetLayoutRegistry,
    std::span<const DescriptorSetLayoutHandle> descriptorSetLayouts,
    DescriptorPool &descriptorPool)
{
    std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
    vkDescriptorSetLayouts.reserve(descriptorSetLayouts.size());

    for (auto descriptorSetLayout : descriptorSetLayouts) {
        const auto &rawDescriptorSetLayout = descriptorSetLayoutRegistry.GetDescriptorSetLayout(descriptorSetLayout);
        vkDescriptorSetLayouts.push_back(rawDescriptorSetLayout.handle);
    }

    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptorPool.handle,
        .descriptorSetCount = static_cast<std::uint32_t>(descriptorSetLayouts.size()),
        .pSetLayouts = vkDescriptorSetLayouts.data()
    };

    std::vector<VkDescriptorSet> vkDescriptorSets(descriptorSetLayouts.size());

    VkResult vkResult = vkAllocateDescriptorSets(logicalDevice.handle, &vkDescriptorSetAllocateInfo, vkDescriptorSets.data());
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Allocate Vulkan Descriptor Sets. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Descriptor Sets Allocated Successfully.\n";

    std::vector<DescriptorSet> descriptorSets;
    descriptorSets.reserve(descriptorSetLayouts.size());

    for (auto vkDescriptorSet : vkDescriptorSets) {
        DescriptorSet descriptorSet {
            .handle = vkDescriptorSet
        };

        descriptorSets.push_back(descriptorSet);
    }

    return descriptorSets;
}

void FreeDescriptorSets(
    VkDevice vkDevice,
    std::span<DescriptorSet> descriptorSets,
    const DescriptorPool &descriptorPool)
{
    if (descriptorSets.empty())
        return;

    std::vector<VkDescriptorSet> vkDescriptorSets;
    vkDescriptorSets.reserve(descriptorSets.size());
    for (const auto &descriptorSet : descriptorSets) {
        if (descriptorSet.handle != VK_NULL_HANDLE)
            vkDescriptorSets.push_back(descriptorSet.handle);
    }

    if (vkDescriptorSets.empty())
        return;

    VkResult vkResult = vkFreeDescriptorSets(
        vkDevice,
        descriptorPool.handle,
        static_cast<std::uint32_t>(vkDescriptorSets.size()),
        vkDescriptorSets.data());

    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Free Vulkan Descriptor Sets. Error Code: {}", static_cast<int>(vkResult)));

    for (auto &descriptorSet : descriptorSets)
        if (descriptorSet.handle != VK_NULL_HANDLE)
            descriptorSet.handle = VK_NULL_HANDLE;
}

}
