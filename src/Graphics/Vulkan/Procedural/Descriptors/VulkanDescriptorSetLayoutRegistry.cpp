#include "VulkanDescriptorSetLayoutRegistry.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>

#include "../Core/VulkanLogicalDevice.hpp"

#include "VulkanDescriptorSetLayout.hpp"
#include "VulkanDescriptorSetLayoutBindings.hpp"

namespace Engine::Graphics::Vulkan {

DescriptorSetLayoutRegistry::DescriptorSetLayoutRegistry(const LogicalDevice &logicalDevice)
    : m_logicalDevice(logicalDevice)
{
}

DescriptorSetLayoutRegistry::~DescriptorSetLayoutRegistry()
{
    Destroy();
}

void DescriptorSetLayoutRegistry::Destroy()
{

    for (auto &descriptorSetLayout : m_descriptorSetLayouts) {
        if (descriptorSetLayout.handle != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(m_logicalDevice.get().handle, descriptorSetLayout.handle, nullptr);
            descriptorSetLayout.handle = VK_NULL_HANDLE;
        }
    }

    m_descriptorSetLayouts.clear();
    m_freeDescriptorSetLayouts.clear();
}

DescriptorSetLayoutHandle DescriptorSetLayoutRegistry::CreateDescriptorSetLayout(const DescriptorSetLayoutBindings &bindings)
{
    VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.vkBindings.size()),
        .pBindings = bindings.vkBindings.data()
    };

    VkDescriptorSetLayout vkHandle = VK_NULL_HANDLE;

    VkResult vkResult = vkCreateDescriptorSetLayout(m_logicalDevice.get().handle, &vkDescriptorSetLayoutCreateInfo, nullptr, &vkHandle);
    if (vkResult != VK_SUCCESS)
        throw std::runtime_error(std::format("Failed to Create Vulkan Descriptor Set Layout. Error Code: {}", static_cast<int>(vkResult)));

    std::cout << "Vulkan Descriptor Set Layout Created Successfully.\n";

    DescriptorSetLayout descriptorSetLayout {
        .handle = vkHandle
    };

    DescriptorSetLayoutHandle handle { };

    if (!m_freeDescriptorSetLayouts.empty()) {
        handle.id = static_cast<uint32_t>(m_freeDescriptorSetLayouts.back());
        m_freeDescriptorSetLayouts.pop_back();
        m_descriptorSetLayouts[handle.id] = descriptorSetLayout;
    } else {
        handle.id = static_cast<uint32_t>(m_descriptorSetLayouts.size());
        m_descriptorSetLayouts.push_back(descriptorSetLayout);
    }

    return handle;
}

void DescriptorSetLayoutRegistry::DestroyDescriptorSetLayout(DescriptorSetLayoutHandle descriptorSetLayout)
{
    auto &rawDescriptorSetLayout = m_descriptorSetLayouts[descriptorSetLayout.id];

    if (rawDescriptorSetLayout.handle != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_logicalDevice.get().handle, rawDescriptorSetLayout.handle, nullptr);
        rawDescriptorSetLayout.handle = VK_NULL_HANDLE;

        m_freeDescriptorSetLayouts.push_back(descriptorSetLayout.id);
    }
}

// Getters
const DescriptorSetLayout &DescriptorSetLayoutRegistry::GetDescriptorSetLayout(DescriptorSetLayoutHandle descriptorSetLayout) const
{
    return m_descriptorSetLayouts[descriptorSetLayout.id];
}

}
