#include "App.hpp"

#include "Graphics/Renderer.hpp"
#include "Window/Window.hpp"

#include <iostream>

App::App() = default;
App::~App() = default;

void App::Run()
{
    Init();
    Loop();
}

void App::Init()
{
    m_window = Engine::CreateWindow(Engine::WindowAPI::GLFW);
    m_renderer = Engine::CreateRenderer(Engine::GraphicsAPI::VULKAN, *m_window);
}

void App::Loop()
{
    while (!m_window->ShouldClose()) {
        m_window->PollEvents();

        m_renderer->Draw();
    }
}
