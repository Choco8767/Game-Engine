#include "App.hpp"

#include <vector>

#include "Assets/AssetFactory.hpp"
#include "Assets/AssetRegistry.hpp"
#include "Graphics/Allocator.hpp"
#include "Graphics/CoreContext.hpp"
#include "Graphics/GraphicsFactory.hpp"
#include "Graphics/RenderContext.hpp"
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
    m_coreContext = Engine::Graphics::CreateCoreContext(Engine::Graphics::API::VULKAN, *m_window);
    m_renderContext = Engine::Graphics::CreateRenderContext(*m_coreContext);
    m_renderer = Engine::Graphics::CreateRenderer(*m_window, *m_coreContext, *m_renderContext);
    m_allocator = Engine::Graphics::CreateAllocator(*m_coreContext);
    m_assets = Engine::Assets::CreateAssetRegistry(*m_allocator);
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

    MeshHandle mesh = m_assets->CreateMesh(vertices, indices);

    while (!m_window->ShouldClose()) {
        m_window->Update();

        if (m_renderer->BeginFrame(*m_window)) {
            m_renderer->DrawMesh(*m_allocator, *m_assets, mesh);
            m_renderer->EndFrame(*m_window);
        }
    }
}
