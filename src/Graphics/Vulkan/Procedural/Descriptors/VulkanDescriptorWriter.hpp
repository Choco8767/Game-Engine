#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics {

enum class DescriptorType;

}

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;
struct DescriptorSet;
class AllocatorBackend;

class DescriptorWriter {
public:
    void Clear();
    void Update(const LogicalDevice &logicalDevice, DescriptorSet targetSet);

    void WriteBuffer(
        const AllocatorBackend &allocator,
        uint32_t binding,
        Graphics::DescriptorType type,
        BufferHandle buffer,
        uint64_t offset, uint64_t range);

private:
    std::vector<VkWriteDescriptorSet> m_writes;

    std::vector<VkDescriptorBufferInfo> m_bufferInfos;
};

}
