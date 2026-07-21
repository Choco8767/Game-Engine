#include "App.hpp"

#include <vector>

#include "Assets/AssetFactory.hpp"
#include "Assets/AssetRegistry.hpp"
#include "Graphics/Context.hpp"
#include "Window/Window.hpp"

#include "Graphics/Core/Vertex.hpp"
#include "Graphics/Types/BufferTypes.hpp"

#include "Graphics/Vulkan/VulkanAllocator.hpp" // Delete After Renderer is Transitioned to Part of an ECS
#include "Graphics/Vulkan/VulkanCoreContext.hpp"
#include "Graphics/Vulkan/VulkanRenderContext.hpp"
#include "Graphics/Vulkan/VulkanRenderer.hpp"

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

    m_graphicsContext = Engine::Graphics::Context::Create(Engine::Graphics::API::VULKAN, *m_window);
    m_renderer = std::make_unique<Engine::Graphics::Vulkan::RendererBackend>( // Delete After Renderer is Transitioned to Part of an ECS
        static_cast<Engine::Graphics::Vulkan::CoreContextBackend &>(m_graphicsContext->GetCoreContext()),
        static_cast<Engine::Graphics::Vulkan::RenderContextBackend &>(m_graphicsContext->GetRenderContext()),
        static_cast<Engine::Graphics::Vulkan::AllocatorBackend &>(m_graphicsContext->GetAllocator()));
    m_renderer->Init(*m_window);
    m_assets = Engine::Assets::CreateAssetRegistry(m_graphicsContext->GetAllocator());
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
            m_renderer->DrawMesh(m_graphicsContext->GetAllocator(), *m_assets, mesh);
            m_renderer->EndFrame(*m_window);
        }
    }
}
