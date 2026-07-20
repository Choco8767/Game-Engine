#pragma once

#include <memory>

#include "Graphics/GraphicsFactory.hpp"

namespace Engine::Graphics::Vulkan {

std::unique_ptr<CoreContext> CreateCoreContext(Engine::Window::Window &window);
std::unique_ptr<RenderContext> CreateRenderContext(const CoreContext &coreContext);
std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, CoreContext &coreContext, RenderContext &renderContext, Allocator &allocator);
std::unique_ptr<Allocator> CreateAllocator(const CoreContext &coreContext);

}
