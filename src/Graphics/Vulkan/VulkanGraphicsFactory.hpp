#pragma once

#include <memory>

#include "Graphics/GraphicsFactory.hpp"

namespace Engine::Graphics::Vulkan {

std::unique_ptr<CoreContext> CreateCoreContext(Engine::Window::Window &window);
std::unique_ptr<RenderContext> CreateRenderContext(const CoreContext &coreContext);
std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, const CoreContext &coreContext, const RenderContext &renderContext);
std::unique_ptr<Allocator> CreateAllocator(const CoreContext &coreContext);

}
