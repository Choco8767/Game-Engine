#include "Renderer.hpp"

#include "GraphicsAPI.hpp"

#include "Vulkan/VulkanRenderer.hpp"

namespace Engine {

std::unique_ptr<Renderer> CreateRenderer(GraphicsAPI api, Window &window)
{
    std::unique_ptr<Renderer> renderer;

    switch (api) {
    case GraphicsAPI::VULKAN:
        renderer = std::make_unique<Vulkan::RendererBackend>();
        break;
    default:
        break;
    }

    if (!renderer)
        return nullptr;

    renderer->Init(window);

    return renderer;
}

}
