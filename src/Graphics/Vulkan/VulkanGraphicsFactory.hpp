#pragma once

#include <memory>

#include "Graphics/GraphicsFactory.hpp"

namespace Engine::Graphics::Vulkan {

std::unique_ptr<Context> CreateContext(Engine::Window::Window &window);
std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, const Context &context);
std::unique_ptr<Allocator> CreateAllocator(const Context &context);

}
