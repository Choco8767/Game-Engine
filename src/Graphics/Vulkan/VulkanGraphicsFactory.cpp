#include "VulkanGraphicsFactory.hpp"

#include "VulkanAllocator.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderer.hpp"

namespace Engine::Graphics::Vulkan {

std::unique_ptr<Context> CreateContext(Engine::Window::Window &window)
{
    auto context = std::make_unique<Vulkan::ContextBackend>();
    if (!context)
        return nullptr;

    context->Init(window);

    return context;
}

std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, const Context &context)
{
    const auto &vulkanContext = static_cast<const Vulkan::ContextBackend &>(context);

    auto renderer = std::make_unique<Vulkan::RendererBackend>(vulkanContext);
    if (!renderer)
        return nullptr;

    renderer->Init(window);

    return renderer;
}

std::unique_ptr<Allocator> CreateAllocator(const Context &context)
{
    const auto &vulkanContext = static_cast<const Vulkan::ContextBackend &>(context);

    auto allocator = std::make_unique<Vulkan::AllocatorBackend>(vulkanContext);
    if (!allocator)
        return nullptr;

    allocator->Init();

    return allocator;
}

}
