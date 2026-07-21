#include "CoreContext.hpp"

#include <format>
#include <stdexcept>

#include "Vulkan/VulkanCoreContext.hpp"

namespace Engine::Graphics {

std::unique_ptr<CoreContext> CoreContext::Create(API api, Engine::Window::Window &window)
{
    switch (api) {
    case API::VULKAN:
        return Vulkan::CoreContextBackend::Create(window);

    default:
        throw std::runtime_error(std::format("Invalid Graphics API Enum: {}", static_cast<int>(api)));
    }
}

}
