#pragma once

#include <memory>

#include "Graphics/API.hpp"

namespace Engine::Graphics {

class CoreContext;

class RenderContext {
public:
    virtual ~RenderContext() = default;

    static std::unique_ptr<RenderContext> Create(const CoreContext &coreContext);
    virtual void Destroy() = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;
};

}
