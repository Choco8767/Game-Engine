#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include <volk.h>

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct DescriptorSet;

class DescriptorSetLayoutRegistry;

struct DescriptorPool {
    VkDescriptorPool handle = VK_NULL_HANDLE;
};

struct DescriptorPoolCreateInfo {
    std::uint32_t maxSets = 0;
    std::uint32_t uniformBufferCount = 0;
    std::uint32_t imageSamplerCount = 0;
};

DescriptorPool CreateDescriptorPool(const LogicalDevice &logicalDevice, const DescriptorPoolCreateInfo &info);
void DestroyDescriptorPool(VkDevice vkDevice, DescriptorPool &descriptorPool);

std::vector<DescriptorSet> AllocateDescriptorSets(
    const LogicalDevice &logicalDevice,
    const DescriptorSetLayoutRegistry &descriptorSetLayoutRegistry,
    std::span<const DescriptorSetLayoutHandle> descriptorSetLayouts,
    DescriptorPool &descriptorPool);

void FreeDescriptorSets(
    VkDevice vkDevice,
    std::span<DescriptorSet> descriptorSets,
    const DescriptorPool &descriptorPool);

}
