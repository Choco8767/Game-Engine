#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

namespace Engine::Vulkan {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() const
    {
        return graphicsFamily.has_value()
            && presentFamily.has_value();
    }
};

}
