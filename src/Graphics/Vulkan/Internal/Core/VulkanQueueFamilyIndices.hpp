#pragma once

#include <cstdint>
#include <optional>

#include <volk.h>

namespace Engine::Graphics::Vulkan {

struct QueueFamilyIndices {
    std::optional<std::uint32_t> graphicsFamily;
    std::optional<std::uint32_t> presentFamily;
    std::optional<std::uint32_t> transferFamily;

    bool IsComplete() const
    {
        return graphicsFamily.has_value()
            && presentFamily.has_value()
            && transferFamily.has_value();
    }
};

}
