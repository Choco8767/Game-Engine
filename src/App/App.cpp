#include "App.hpp"

#include <vector>

#include "Graphics/Allocator.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/GraphicsFactory.hpp"
#include "Graphics/Renderer.hpp"
#include "Window/Window.hpp"

#include "Graphics/Core/Vertex.hpp"
#include "Graphics/Types/BufferTypes.hpp"

App::App() = default;
App::~App() = default;

void App::Run()
{
    Init();
    Loop();
}

void App::Init()
{
    m_window = Engine::Window::CreateWindow(Engine::Window::API::GLFW);
    m_context = Engine::Graphics::CreateContext(Engine::Graphics::API::VULKAN, *m_window);
    m_renderer = Engine::Graphics::CreateRenderer(*m_window, *m_context);
    m_allocator = Engine::Graphics::CreateAllocator(*m_context);
}

void App::Loop()
{
    const std::vector<Engine::Graphics::Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } }
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };

    Engine::Graphics::BufferCreateInfo vertexBufferCreateInfo {
        .size = vertices.size() * sizeof(Engine::Graphics::Vertex),
        .usage = Engine::Graphics::BufferUsage::VERTEX
    };
    Engine::Graphics::BufferCreateInfo indexBufferCreateInfo {
        .size = indices.size() * sizeof(uint32_t),
        .usage = Engine::Graphics::BufferUsage::INDEX
    };

    auto vertexBuffer = m_allocator->CreateBuffer(vertexBufferCreateInfo, vertices.data());
    auto indexBuffer = m_allocator->CreateBuffer(indexBufferCreateInfo, indices.data());

    while (!m_window->ShouldClose()) {
        m_window->Update();

        if (m_renderer->BeginFrame(*m_window)) {
            m_renderer->DrawMesh(*m_allocator, vertexBuffer, indexBuffer, indices.size());
            m_renderer->EndFrame(*m_window);
        }
    }
}
