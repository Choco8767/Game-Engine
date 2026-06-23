#pragma once

#include "GraphicsAPI.hpp"

namespace Engine::Graphics {

class Context;

class RenderContext {
public:
    virtual ~RenderContext() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
