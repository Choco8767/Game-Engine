#pragma once

#include <memory>

#include <vulkan/vulkan.h>

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class Context;

}

namespace Engine::Graphics::Vulkan {

class RendererBackend;

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
    std::unique_ptr<Engine::Graphics::Context> m_graphicsContext;
    std::unique_ptr<Engine::Graphics::Vulkan::RendererBackend> m_renderer;
    std::unique_ptr<Engine::Assets::AssetRegistry> m_assets;
};
