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

std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, CoreContext &coreContext, RenderContext &renderContext, Allocator &allocator)
{
    auto &vulkanCoreContext = static_cast<Vulkan::CoreContextBackend &>(coreContext);
    auto &vulkanRenderContext = static_cast<Vulkan::RenderContextBackend &>(renderContext);
    auto &vulkanAllocator = static_cast<Vulkan::AllocatorBackend &>(allocator);

    auto renderer = std::make_unique<Vulkan::RendererBackend>(vulkanCoreContext, vulkanRenderContext, vulkanAllocator);
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
