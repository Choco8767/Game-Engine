#include "App.hpp"

#include <vector>

#include "Assets/AssetFactory.hpp"
#include "Assets/AssetRegistry.hpp"
#include "Window/Window.hpp"

#include "Graphics/Core/Vertex.hpp"

#include "Graphics/Context/Context.hpp"
#include "Graphics/Renderer.hpp"

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

    const MeshHandle mesh = m_assets->CreateMesh(vertices, indices);

    std::vector<Engine::Assets::MeshAsset> renderList;
    renderList.reserve(1);

    auto &renderer = m_graphicsContext->GetRenderer();

    while (!m_window->ShouldClose()) {
        m_window->Update();

        renderList.clear();
        renderList.push_back(m_assets->GetMesh(mesh));

        if (renderer.BeginFrame(*m_window)) {
            renderer.DrawMeshes(renderList);
            renderer.EndFrame(*m_window);
        }
    }
}
