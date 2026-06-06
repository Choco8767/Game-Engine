#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Graphics/Core/Vertex.hpp"

namespace Engine::Graphics::Vulkan {

VkVertexInputBindingDescription GetVertexBindingDescription();
std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();

}
