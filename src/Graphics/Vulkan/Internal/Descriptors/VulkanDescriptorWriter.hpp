#pragma once

#include <cstdint>
#include <vector>

#include <volk.h>

#include "Graphics/Types/DescriptorTypes.hpp"

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct DescriptorSet;
struct Buffer;

class DescriptorWriter {
public:
    void Clear();
    void Update(const LogicalDevice &logicalDevice, DescriptorSet targetSet);

    void WriteBuffer(
        const Buffer &buffer,
        std::uint32_t binding,
        Graphics::DescriptorType type,
        std::uint64_t offset,
        std::uint64_t range);

private:
    std::vector<VkWriteDescriptorSet> m_writes;

    std::vector<VkDescriptorBufferInfo> m_bufferInfos;
};

}
