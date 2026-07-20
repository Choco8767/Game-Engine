#pragma once

#include <memory>

#include "Graphics/GraphicsAPI.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class CoreContext;
class RenderContext;
class Allocator;
class Renderer;

std::unique_ptr<CoreContext> CreateCoreContext(API api, Engine::Window::Window &window);
std::unique_ptr<RenderContext> CreateRenderContext(const CoreContext &coreContext);
std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, CoreContext &coreContext, RenderContext &renderContext, Allocator &allocator);
std::unique_ptr<Allocator> CreateAllocator(const CoreContext &coreContext);

}
