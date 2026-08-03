#pragma once

#include <vector>

#include <volk.h>

#include "Graphics/Core/Vertex.hpp"

namespace Engine::Graphics::Vulkan {

VkVertexInputBindingDescription GetVertexBindingDescription();
std::vector<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();

}
