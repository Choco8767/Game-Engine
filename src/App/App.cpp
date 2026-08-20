#include "App.hpp"

#include <vector>

#include "Assets/AssetFactory.hpp"
#include "Assets/AssetRegistry.hpp"
#include "Window/Window.hpp"

#include "Graphics/Core/Vertex.hpp"

#include "Graphics/Context/AllocatorContext.hpp"
#include "Graphics/Context/Context.hpp"

#include "Graphics/Vulkan/Allocators/VulkanBufferAllocator.hpp"
#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"
#include "Graphics/Vulkan/Context/VulkanRenderContext.hpp"
#include "Graphics/Vulkan/VulkanRenderer.hpp" // Delete After Renderer is Transitioned to Part of an ECS

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
        static_cast<Engine::Graphics::Vulkan::BufferAllocatorBackend &>(m_graphicsContext->GetAllocatorContext().GetBufferAllocator()));
    m_renderer->Init(*m_window);
    m_assets = Engine::Assets::CreateAssetRegistry(m_graphicsContext->GetAllocatorContext());
}

void App::Loop()
{
    const std::vector<Engine::Graphics::Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } }
    };

    const std::vector<std::uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };

    MeshHandle mesh = m_assets->CreateMesh(vertices, indices);

    while (!m_window->ShouldClose()) {
        m_window->Update();

        if (m_renderer->BeginFrame(*m_window)) {
            m_renderer->DrawMesh(*m_assets, mesh);
            m_renderer->EndFrame(*m_window);
        }
    }
}
