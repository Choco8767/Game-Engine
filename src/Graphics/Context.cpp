#include "Graphics/Context.hpp"

#include <format>
#include <stdexcept>

#include "Allocator.hpp"
#include "CoreContext.hpp"
#include "RenderContext.hpp"

#include "Vulkan/VulkanAllocator.hpp"

namespace Engine::Graphics {

Context::Context(
    Passkey<Context>,
    std::unique_ptr<CoreContext> coreContext,
    std::unique_ptr<RenderContext> renderContext,
    std::unique_ptr<Allocator> allocator)
    : m_coreContext(std::move(coreContext))
    , m_renderContext(std::move(renderContext))
    , m_allocator(std::move(allocator))
{
}

std::unique_ptr<Context> Context::Create(API api, Window::Window &window)
{
    auto coreContext = CoreContext::Create(api, window);
    auto renderContext = RenderContext::Create(*coreContext);
    auto allocator = Allocator::Create(*coreContext);

    return std::make_unique<Context>(
        Passkey<Context> { },
        std::move(coreContext),
        std::move(renderContext),
        std::move(allocator));
}

}
