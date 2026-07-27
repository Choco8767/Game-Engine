#pragma once

#include <memory>

#include "Graphics/API.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class CoreContext {
public:
    virtual ~CoreContext() = default;

    static std::unique_ptr<CoreContext> Create(API api, Engine::Window::Window &window);
    virtual void Destroy() = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
