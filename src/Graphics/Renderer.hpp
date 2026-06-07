#pragma once

#include <cstdint>

#include "Assets/Types/AssetHandles.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Assets {

class AssetRegistry;

}

namespace Engine::Graphics {

class Allocator;

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void Init(Window::Window &window) = 0;
    virtual void Destroy() = 0;

    virtual bool BeginFrame(const Window::Window &window) = 0;
    virtual void EndFrame(const Window::Window &window) = 0;

    virtual void DrawMesh(
        const Allocator &allocator,
        const Assets::AssetRegistry &assets,
        MeshHandle mesh,
        uint32_t instanceCount = 1,
        uint32_t firstIndex = 0,
        int32_t vertexOffset = 0,
        uint32_t firstInstance = 0) = 0;
};

}
