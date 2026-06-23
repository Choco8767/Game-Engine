#pragma once

#include <memory>

#include <vulkan/vulkan.h>

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class CoreContext;
class RenderContext;
class Renderer;
class Allocator;

}

namespace Engine::Assets {

class AssetRegistry;

}

class App {
public:
    App();
    ~App();

    void Run();

private:
    void Init();
    void Loop();

    std::unique_ptr<Engine::Window::Window> m_window;
    std::unique_ptr<Engine::Graphics::CoreContext> m_coreContext;
    std::unique_ptr<Engine::Graphics::RenderContext> m_renderContext;
    std::unique_ptr<Engine::Graphics::Renderer> m_renderer;
    std::unique_ptr<Engine::Graphics::Allocator> m_allocator;
    std::unique_ptr<Engine::Assets::AssetRegistry> m_assets;
};
