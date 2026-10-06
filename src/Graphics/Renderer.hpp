#pragma once

#include <memory>
#include <span>

#include "Assets/Types/MeshAsset.hpp"
#include "Graphics/API.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class CoreContext;
class RenderContext;
class AllocatorContext;

class Renderer {
public:
    virtual ~Renderer() = default;

    static std::unique_ptr<Renderer> Create(
        const Window::Window &window,
        CoreContext &coreContext,
        RenderContext &renderContext,
        AllocatorContext &allocatorContext);

    virtual void Destroy() = 0;

    // Getters
    virtual API GetAPIType() const noexcept = 0;

    virtual bool BeginFrame(const Window::Window &window) = 0;
    virtual void EndFrame(const Window::Window &window) = 0;
    virtual void DrawMeshes(std::span<const Assets::MeshAsset> meshes) = 0;
};

}
