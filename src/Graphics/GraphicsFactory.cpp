#include "GraphicsFactory.hpp"

#include "Allocator.hpp"
#include "Context.hpp"
#include "Renderer.hpp"

#include "Vulkan/VulkanGraphicsFactory.hpp"

namespace Engine::Graphics {

std::unique_ptr<Context> CreateContext(API api, Engine::Window::Window &window)
{
    switch (api) {
    case API::VULKAN:
        return Vulkan::CreateContext(window);
    default:
        return nullptr;
    }
}

std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, const Context &context)
{
    switch (context.GetAPIType()) {
    case API::VULKAN:
        return Vulkan::CreateRenderer(window, context);
    default:
        return nullptr;
    }
}

std::unique_ptr<Allocator> CreateAllocator(const Context &context)
{
    switch (context.GetAPIType()) {
    case API::VULKAN:
        return Vulkan::CreateAllocator(context);
    default:
        return nullptr;
    }
}

}
