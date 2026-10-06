#include "AllocatorContext.hpp"

#include <stdexcept>
#include <format>

#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"
#include "Graphics/Vulkan/Context/VulkanAllocatorContext.hpp"

namespace Engine::Graphics {

AllocatorContext::~AllocatorContext() = default;

std::unique_ptr<AllocatorContext> AllocatorContext::Create(const CoreContext &coreContext)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN: {
        const auto& vulkanCoreContext = static_cast<const Vulkan::CoreContextBackend&>(coreContext);
        return Vulkan::AllocatorContextBackend::Create(vulkanCoreContext);
    }

    default:
        throw std::runtime_error(std::format("Invalid Graphics API Enum: {}", static_cast<int>(coreContext.GetAPIType())));
    }
}

}
