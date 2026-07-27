#include "VulkanCoreContext.hpp"

#include "Window/Window.hpp"

namespace Engine::Graphics::Vulkan {

CoreContextBackend::CoreContextBackend(
    Passkey<CoreContextBackend>,
    Instance instance,
    Surface surface,
    PhysicalDevice physicalDevice,
    LogicalDevice logicalDevice,
    MemoryAllocator memoryAllocator)
    : m_instance(std::move(instance))
    , m_surface(std::move(surface))
    , m_physicalDevice(std::move(physicalDevice))
    , m_logicalDevice(std::move(logicalDevice))
    , m_memoryAllocator(std::move(memoryAllocator))
{
}

CoreContextBackend::~CoreContextBackend()
{
    Destroy();
}

std::unique_ptr<CoreContextBackend> CoreContextBackend::Create(Engine::Window::Window &window)
{
    auto requiredExtensions = window.GetRequiredInstanceExtensions();

    auto instance = Vulkan::CreateInstance(requiredExtensions);
    auto surface = Vulkan::CreateSurface(instance, window);
    auto physicalDevice = Vulkan::CreatePhysicalDevice(instance, surface);
    PopulateSurfaceDetails(physicalDevice, surface);
    auto logicalDevice = Vulkan::CreateLogicalDevice(physicalDevice);
    auto memoryAllocator = CreateMemoryAllocator(instance, physicalDevice, logicalDevice);

    return std::make_unique<CoreContextBackend>(
        Passkey<CoreContextBackend> { },
        std::move(instance),
        std::move(surface),
        std::move(physicalDevice),
        std::move(logicalDevice),
        std::move(memoryAllocator));
}

void CoreContextBackend::Destroy()
{
    WaitIdle(m_logicalDevice);

    Vulkan::DestroyMemoryAllocator(m_logicalDevice, m_memoryAllocator);
    Vulkan::DestroyLogicalDevice(m_logicalDevice);
    Vulkan::DestroyPhysicalDevice(m_physicalDevice);
    Vulkan::DestroySurface(m_instance.handle, m_surface);
    Vulkan::DestroyInstance(m_instance);
}

}
