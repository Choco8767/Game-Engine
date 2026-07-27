#include "VulkanDescriptorWriter.hpp"

#include "Graphics/Vulkan/Internal/Resources/VulkanBuffer.hpp"

#include "Graphics/Vulkan/Helpers/VulkanDescriptorTypes.hpp"

#include "../Core/VulkanLogicalDevice.hpp"
#include "../Resources/VulkanBuffer.hpp"

#include "VulkanDescriptorSet.hpp"

namespace Engine::Graphics::Vulkan {

void DescriptorWriter::Clear()
{
    m_writes.clear();
    m_bufferInfos.clear();
}

void DescriptorWriter::Update(const LogicalDevice &logicalDevice, DescriptorSet targetSet)
{
    for (auto &write : m_writes)
        write.dstSet = targetSet.handle;

    vkUpdateDescriptorSets(
        logicalDevice.handle,
        static_cast<uint32_t>(m_writes.size()),
        m_writes.data(),
        0, nullptr);

    Clear();
}

void DescriptorWriter::WriteBuffer(
    const Buffer &buffer,
    uint32_t binding,
    Graphics::DescriptorType type,
    uint64_t offset,
    uint64_t range)
{
    VkDescriptorBufferInfo bufferInfo {
        .buffer = buffer.handle,
        .offset = offset,
        .range = range == 0 ? buffer.allocation.size : range
    };

    VkWriteDescriptorSet vkWriteDescriptorSet {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstBinding = binding,
        .descriptorCount = 1,
        .descriptorType = MapDescriptorType(type),
        .pBufferInfo = &m_bufferInfos.emplace_back(bufferInfo)
    };

    m_writes.push_back(vkWriteDescriptorSet);
}

}
