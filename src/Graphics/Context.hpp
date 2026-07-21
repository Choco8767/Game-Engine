#pragma once

#include <memory>

#include "API.hpp"
#include "Utils/Passkey.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class CoreContext;
class RenderContext;
class Allocator;

class Context {
public:
    Context(
        Passkey<Context>,
        std::unique_ptr<CoreContext> coreContext,
        std::unique_ptr<RenderContext> renderContext,
        std::unique_ptr<Allocator> allocator);
    ~Context() = default;

    Context(const Context &other) = delete;
    Context &operator=(const Context &other) = delete;

    Context(Context &&other) noexcept = default;
    Context &operator=(Context &&other) noexcept = default;

    static std::unique_ptr<Context> Create(API api, Window::Window &window);

    // Getters
    CoreContext &GetCoreContext() { return *m_coreContext; }
    const CoreContext &GetCoreContext() const { return *m_coreContext; }
    RenderContext &GetRenderContext() { return *m_renderContext; }
    const RenderContext &GetRenderContext() const { return *m_renderContext; }
    Allocator &GetAllocator() { return *m_allocator; }
    const Allocator &GetAllocator() const { return *m_allocator; }

private:
    std::unique_ptr<CoreContext> m_coreContext;
    std::unique_ptr<RenderContext> m_renderContext;
    std::unique_ptr<Allocator> m_allocator;
};

}
