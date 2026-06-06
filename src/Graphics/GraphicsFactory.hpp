#pragma once

#include <memory>

#include "Graphics/GraphicsAPI.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics {

class Context;
class Renderer;
class Allocator;

std::unique_ptr<Context> CreateContext(API api, Engine::Window::Window &window);
std::unique_ptr<Renderer> CreateRenderer(Engine::Window::Window &window, const Context &context);
std::unique_ptr<Allocator> CreateAllocator(const Context &context);

}
