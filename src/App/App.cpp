#include "App.hpp"

#include "Window/Window.hpp"

#include <stdexcept>

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
}

void App::Loop()
{
    while (!m_window->ShouldClose()) {
        m_window->PollEvents();
    }
}
