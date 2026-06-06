#pragma once

#include <cstdint>

#include "Types/ResourceHandles.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class Allocator;

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void Init(Engine::Window::Window &window) = 0;
    virtual void Destroy() = 0;

    virtual bool BeginFrame(const Engine::Window::Window &window) = 0;
    virtual void EndFrame(const Engine::Window::Window &window) = 0;

    virtual void DrawMesh(
        const Allocator &allocator,
        BufferHandle vertexBuffer,
        BufferHandle indexBuffer,
        uint32_t indexCount,
        uint32_t instanceCount = 1,
        uint32_t firstIndex = 0,
        int32_t vertexOffset = 0,
        uint32_t firstInstance = 0) = 0;
};

}
