#pragma once

#include <vector>

#include <volk.h>

namespace Engine::Graphics::Vulkan {

VkVertexInputBindingDescription GetVertexBindingDescription();
std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();

}
