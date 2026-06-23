#pragma once

#include "GraphicsAPI.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class CoreContext {
public:
    virtual ~CoreContext() = default;

    virtual void Init(Engine::Window::Window &window) = 0;
    virtual void Destroy() = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
