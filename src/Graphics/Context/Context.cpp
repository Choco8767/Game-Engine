#include "Context.hpp"

#include <format>
#include <stdexcept>

#include "AllocatorContext.hpp"
#include "CoreContext.hpp"
#include "RenderContext.hpp"

namespace Engine::Graphics {

Context::Context(
    Passkey<Context>,
    std::unique_ptr<CoreContext> coreContext,
    std::unique_ptr<RenderContext> renderContext,
    std::unique_ptr<AllocatorContext> allocatorContext)
    : m_coreContext(std::move(coreContext))
    , m_renderContext(std::move(renderContext))
    , m_allocatorContext(std::move(allocatorContext))
{
}

std::unique_ptr<Context> Context::Create(API api, Window::Window &window)
{
    auto coreContext = CoreContext::Create(api, window);
    auto renderContext = RenderContext::Create(*coreContext);
    auto allocatorContext = AllocatorContext::Create(*coreContext);

    return std::make_unique<Context>(
        Passkey<Context> { },
        std::move(coreContext),
        std::move(renderContext),
        std::move(allocatorContext));
}

}
