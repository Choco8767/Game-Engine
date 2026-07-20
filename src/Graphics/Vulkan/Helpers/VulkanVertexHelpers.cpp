#include "VulkanVertexHelpers.hpp"

namespace Engine::Graphics::Vulkan {

VkVertexInputBindingDescription GetVertexBindingDescription()
{
    VkVertexInputBindingDescription vkVertexInputBindingDescription {
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return vkVertexInputBindingDescription;
}

std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> vkVertexAttributeDescriptions {
        {
            // Position
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, position),
        },
        {
            // Color
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(Vertex, color),
        }
    };

    return vkVertexAttributeDescriptions;
}

}
