#pragma once

#include <vulkan/vulkan.h>

#include "../CoreContext.hpp"

#include "Procedural/Core/VulkanInstance.hpp"
#include "Procedural/Core/VulkanLogicalDevice.hpp"
#include "Procedural/Core/VulkanPhysicalDevice.hpp"
#include "Procedural/Core/VulkanSurface.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend final : public Engine::Graphics::CoreContext {
public:
    CoreContextBackend() = default;
    ~CoreContextBackend() override;

    void Init(Engine::Window::Window &window) override;
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
