#include "Allocator.hpp"

#include "Vulkan/VulkanCoreContext.hpp"

#include "Vulkan/VulkanAllocator.hpp"

#include <format>
#include <stdexcept>

namespace Engine::Graphics {

std::unique_ptr<Allocator> Allocator::Create(const CoreContext &coreContext)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN: {
        const auto &vulkanCoreContext = static_cast<const Vulkan::CoreContextBackend &>(coreContext);
        return Vulkan::AllocatorBackend::Create(vulkanCoreContext);
    }

    default:
        throw std::runtime_error(std::format("Invalid Graphics API Enum: {}", static_cast<int>(coreContext.GetAPIType())));
    }
}

}
