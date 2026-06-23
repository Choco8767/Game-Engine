#include "VulkanGraphicsFactory.hpp"

#include "VulkanAllocator.hpp"
#include "VulkanCoreContext.hpp"
#include "VulkanRenderContext.hpp"
#include "VulkanRenderer.hpp"

namespace Engine::Graphics::Vulkan {

std::unique_ptr<CoreContext> CreateCoreContext(Engine::Window::Window &window)
{
    auto coreContext = std::make_unique<Vulkan::CoreContextBackend>();
    if (!coreContext)
        return nullptr;

    coreContext->Init(window);

    return coreContext;
}

std::unique_ptr<RenderContext> CreateRenderContext(const CoreContext &coreContext)
{
    const auto &vulkanCoreContext = static_cast<const Vulkan::CoreContextBackend &>(coreContext);

    auto renderContext = std::make_unique<Vulkan::RenderContextBackend>(vulkanCoreContext);
    if (!renderContext)
        return nullptr;

    renderContext->Init();

    return renderContext;
}

std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, const CoreContext &coreContext, const RenderContext &renderContext)
{
    const auto &vulkanCoreContext = static_cast<const Vulkan::CoreContextBackend &>(coreContext);
    const auto &vulkanRenderContext = static_cast<const Vulkan::RenderContextBackend &>(renderContext);

    auto renderer = std::make_unique<Vulkan::RendererBackend>(vulkanCoreContext, vulkanRenderContext);
    if (!renderer)
        return nullptr;

    renderer->Init(window);

    return renderer;
}

std::unique_ptr<Allocator> CreateAllocator(const CoreContext &coreContext)
{
    const auto &vulkanCoreContext = static_cast<const Vulkan::CoreContextBackend &>(coreContext);

    auto allocator = std::make_unique<Vulkan::AllocatorBackend>(vulkanCoreContext);
    if (!allocator)
        return nullptr;

    allocator->Init();

    return allocator;
}

}
