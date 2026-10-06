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
    for (std::size_t i = 0; i < m_writes.size(); i++) {
        m_writes[i].dstSet = targetSet.handle;
        m_writes[i].pBufferInfo = &m_bufferInfos[i];
    }

    vkUpdateDescriptorSets(
        logicalDevice.handle,
        static_cast<std::uint32_t>(m_writes.size()),
        m_writes.data(),
        0, nullptr);

    Clear();
}

void DescriptorWriter::WriteBuffer(
    const Buffer &buffer,
    std::uint32_t binding,
    Graphics::DescriptorType type,
    std::uint64_t offset,
    std::uint64_t range)
{
    VkDescriptorBufferInfo bufferInfo {
        .buffer = buffer.handle,
        .offset = offset,
        .range = range == 0 ? buffer.allocation.size : range
    };

    m_bufferInfos.emplace_back(bufferInfo);

    VkWriteDescriptorSet vkWriteDescriptorSet {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstBinding = binding,
        .descriptorCount = 1,
        .descriptorType = MapDescriptorType(type),
    };

    m_writes.push_back(vkWriteDescriptorSet);
}

}
