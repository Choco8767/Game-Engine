#include "GraphicsFactory.hpp"

#include "Allocator.hpp"
#include "CoreContext.hpp"
#include "RenderContext.hpp"
#include "Renderer.hpp"

#include "Vulkan/VulkanGraphicsFactory.hpp"

namespace Engine::Graphics {

std::unique_ptr<CoreContext> CreateCoreContext(API api, Engine::Window::Window &window)
{
    switch (api) {
    case API::VULKAN:
        return Vulkan::CreateCoreContext(window);
    default:
        return nullptr;
    }
}

std::unique_ptr<RenderContext> CreateRenderContext(const CoreContext &coreContext)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN:
        return Vulkan::CreateRenderContext(coreContext);
    default:
        return nullptr;
    }
}

std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, CoreContext &coreContext, RenderContext &renderContext, Allocator &allocator)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN:
        return Vulkan::CreateRenderer(window, coreContext, renderContext, allocator);
    default:
        return nullptr;
    }
}

std::unique_ptr<Allocator> CreateAllocator(const CoreContext &coreContext)
{
    switch (coreContext.GetAPIType()) {
    case API::VULKAN:
        return Vulkan::CreateAllocator(coreContext);
    default:
        return nullptr;
    }
}

}
