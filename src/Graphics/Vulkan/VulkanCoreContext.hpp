#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "Utils/Passkey.hpp"

#include "../CoreContext.hpp"

#include "Procedural/Core/VulkanInstance.hpp"
#include "Procedural/Core/VulkanLogicalDevice.hpp"
#include "Procedural/Core/VulkanPhysicalDevice.hpp"
#include "Procedural/Core/VulkanSurface.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend final : public Engine::Graphics::CoreContext {
public:
    CoreContextBackend(
        Passkey<CoreContextBackend>,
        Instance instance,
        Surface surface,
        PhysicalDevice physicalDevice,
        LogicalDevice logicalDevice);
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

private:
    Instance m_instance;
    Surface m_surface;
    PhysicalDevice m_physicalDevice;
    LogicalDevice m_logicalDevice;
};

}
