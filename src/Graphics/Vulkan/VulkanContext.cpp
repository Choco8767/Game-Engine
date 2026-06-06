#include "VulkanContext.hpp"

#include "Window/Window.hpp"

namespace Engine::Graphics::Vulkan {

ContextBackend::~ContextBackend()
{
    Destroy();
}

void ContextBackend::Init(Engine::Window::Window &window)
{
    auto requiredExtensions = window.GetRequiredInstanceExtensions();

    m_instance = Vulkan::CreateInstance(requiredExtensions);
    m_surface = Vulkan::CreateSurface(m_instance, window);
    m_physicalDevice = Vulkan::CreatePhysicalDevice(m_instance, m_surface);
    m_logicalDevice = Vulkan::CreateLogicalDevice(m_physicalDevice);
}

void ContextBackend::Destroy()
{
    WaitIdle(m_logicalDevice);

    Vulkan::DestroyLogicalDevice(m_logicalDevice);
    Vulkan::DestroyPhysicalDevice(m_physicalDevice);
    Vulkan::DestroySurface(m_instance.handle, m_surface);
    Vulkan::DestroyInstance(m_instance);
}

}
