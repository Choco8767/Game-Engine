#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.h>

namespace Engine {

class Window;
class Renderer;

}

class App {
public:
    App();
    ~App();

    void Run();

private:
    void Init();
    void Loop();

    std::unique_ptr<Engine::Window> m_window;
    std::unique_ptr<Engine::Renderer> m_renderer;
};
