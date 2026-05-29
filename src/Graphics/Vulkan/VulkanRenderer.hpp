#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "../Renderer.hpp"

#include "Procedural/Commands/VulkanCommandBuffer.hpp"
#include "Procedural/Commands/VulkanCommandPool.hpp"
#include "Procedural/Core/VulkanInstance.hpp"
#include "Procedural/Core/VulkanLogicalDevice.hpp"
#include "Procedural/Core/VulkanPhysicalDevice.hpp"
#include "Procedural/Core/VulkanSurface.hpp"
#include "Procedural/Pipeline/VulkanGraphicsPipeline.hpp"
#include "Procedural/Rendering/VulkanRenderPass.hpp"
#include "Procedural/Swapchain/VulkanSwapchain.hpp"
#include "Procedural/Sync/VulkanFence.hpp"
#include "Procedural/Sync/VulkanSemaphore.hpp"

namespace Engine::Vulkan {

struct FrameData {
    CommandBuffer commandBuffer { };

    Semaphore imageAvailableSemaphore { };
    Semaphore renderFinishedSemaphore { };
    Fence inFlightFence { };
};

class RendererBackend final : public Engine::Renderer {
public:
    RendererBackend() = default;
    ~RendererBackend() override;

    void Init(Window &window) override;
    void Destroy() override;

    void Draw() override;

private:
    Instance m_instance;
    Surface m_surface;
    PhysicalDevice m_physicalDevice;
    LogicalDevice m_logicalDevice;
    Swapchain m_swapchain;
    RenderPass m_renderPass;
    GraphicsPipeline m_graphicsPipeline;
    CommandPool m_commandPool;

    std::vector<FrameData> m_frames;
    uint32_t m_currentFrame = 0;
};

}
