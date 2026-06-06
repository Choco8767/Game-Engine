#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "../Context.hpp"

#include "Procedural/Core/VulkanInstance.hpp"
#include "Procedural/Core/VulkanLogicalDevice.hpp"
#include "Procedural/Core/VulkanPhysicalDevice.hpp"
#include "Procedural/Core/VulkanSurface.hpp"

namespace Engine::Graphics::Vulkan {

class ContextBackend final : public Engine::Graphics::Context {
public:
    ContextBackend() = default;
    ~ContextBackend() override;

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
