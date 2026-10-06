#include "Renderer.hpp"

#include <format>
#include <stdexcept>

#include "Graphics/Context/AllocatorContext.hpp"
#include "Graphics/Context/CoreContext.hpp"
#include "Graphics/Context/RenderContext.hpp"

#include "Graphics/Vulkan/Context/VulkanAllocatorContext.hpp"
#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"
#include "Graphics/Vulkan/Context/VulkanRenderContext.hpp"

#include "Graphics/Vulkan/VulkanRenderer.hpp"

namespace Engine::Graphics {

std::unique_ptr<Renderer> Renderer::Create(
    const Window::Window &window,
    CoreContext &coreContext,
    RenderContext &renderContext,
    AllocatorContext &allocatorContext)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN: {
        auto &vulkanCoreContext = static_cast<Vulkan::CoreContextBackend &>(coreContext);
        auto &vulkanRenderContext = static_cast<Vulkan::RenderContextBackend &>(renderContext);
        auto &vulkanAllocatorContext = static_cast<Vulkan::AllocatorContextBackend &>(allocatorContext);
        return Vulkan::RendererBackend::Create(window, vulkanCoreContext, vulkanRenderContext, vulkanAllocatorContext);
    }

    default:
        throw std::runtime_error(std::format("Invalid Graphics API Enum: {}", static_cast<int>(coreContext.GetAPIType())));
    }
}

}
