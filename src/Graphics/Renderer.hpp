#pragma once

#include <memory>

#include "GraphicsAPI.hpp"

namespace Engine {

class Window;

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void Init(Window &window) = 0;
    virtual void Destroy() = 0;

    virtual void Draw() = 0;
};

std::unique_ptr<Renderer> CreateRenderer(GraphicsAPI api, Window &window);

}
