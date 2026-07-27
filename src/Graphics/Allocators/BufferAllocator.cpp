#include "BufferAllocator.hpp"

#include <format>
#include <stdexcept>

#include "Graphics/Context/CoreContext.hpp"

#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"

#include "Graphics/Vulkan/Allocators/VulkanBufferAllocator.hpp"

namespace Engine::Graphics {

std::unique_ptr<BufferAllocator> BufferAllocator::Create(const CoreContext &coreContext)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN: {
        const auto &vulkanCoreContext = static_cast<const Vulkan::CoreContextBackend &>(coreContext);
        return Vulkan::BufferAllocatorBackend::Create(vulkanCoreContext);
    }

    default:
        throw std::runtime_error(std::format("Invalid Graphics API Enum: {}", static_cast<int>(coreContext.GetAPIType())));
    }
}

}
