#include "Context.hpp"

#include "AllocatorContext.hpp"
#include "CoreContext.hpp"
#include "Graphics/Renderer.hpp"
#include "RenderContext.hpp"

namespace Engine::Graphics {

Context::~Context() = default;

Context::Context(
    Passkey<Context>,
    std::unique_ptr<CoreContext> coreContext,
    std::unique_ptr<RenderContext> renderContext,
    std::unique_ptr<AllocatorContext> allocatorContext,
    std::unique_ptr<Renderer> renderer)
    : m_coreContext(std::move(coreContext))
    , m_renderContext(std::move(renderContext))
    , m_allocatorContext(std::move(allocatorContext))
    , m_renderer(std::move(renderer))
{
}

std::unique_ptr<Context> Context::Create(API api, Window::Window &window)
{
    auto coreContext = CoreContext::Create(api, window);
    auto renderContext = RenderContext::Create(*coreContext);
    auto allocatorContext = AllocatorContext::Create(*coreContext);
    auto renderer = Renderer::Create(window, *coreContext, *renderContext, *allocatorContext);

    return std::make_unique<Context>(
        Passkey<Context> {},
        std::move(coreContext),
        std::move(renderContext),
        std::move(allocatorContext),
        std::move(renderer));
}

}
