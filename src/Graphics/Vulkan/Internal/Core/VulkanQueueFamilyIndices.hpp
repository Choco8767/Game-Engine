#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Graphics::Vulkan {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> transferFamily;

    bool IsComplete() const
    {
        return graphicsFamily.has_value()
            && presentFamily.has_value()
            && transferFamily.has_value();
    }
};

}
