#pragma once

#include <memory>

#include <volk.h>

#include "Utils/Passkey.hpp"

#include "Graphics/Context/CoreContext.hpp"

#include "Graphics/Vulkan/Internal/Core/VulkanInstance.hpp"
#include "Graphics/Vulkan/Internal/Core/VulkanLogicalDevice.hpp"
#include "Graphics/Vulkan/Internal/Core/VulkanPhysicalDevice.hpp"
#include "Graphics/Vulkan/Internal/Core/VulkanSurface.hpp"
#include "Graphics/Vulkan/Internal/Memory/VulkanMemoryAllocator.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend final : public Engine::Graphics::CoreContext {
public:
    CoreContextBackend(
        Passkey<CoreContextBackend>,
        Instance instance,
        Surface surface,
        PhysicalDevice physicalDevice,
        LogicalDevice logicalDevice,
        MemoryAllocator memoryAllocator);
    ~CoreContextBackend() override;

    CoreContextBackend(const CoreContextBackend &other) = delete;
    CoreContextBackend &operator=(const CoreContextBackend &other) = delete;

    CoreContextBackend(CoreContextBackend &&other) noexcept = default;
    CoreContextBackend &operator=(CoreContextBackend &&other) noexcept = default;

    static std::unique_ptr<CoreContextBackend> Create(Engine::Window::Window &window);
    void Destroy() override;

    // Getters
    API GetAPIType() const noexcept override { return API::VULKAN; }

    const Instance &GetInstance() const { return m_instance; }
    const Surface &GetSurface() const { return m_surface; }
    const PhysicalDevice &GetPhysicalDevice() const { return m_physicalDevice; }
    const LogicalDevice &GetLogicalDevice() const { return m_logicalDevice; }
    const MemoryAllocator &GetMemoryAllocator() const { return m_memoryAllocator; }

private:
    Instance m_instance;
    Surface m_surface;
    PhysicalDevice m_physicalDevice;
    LogicalDevice m_logicalDevice;
    MemoryAllocator m_memoryAllocator;
};

}
