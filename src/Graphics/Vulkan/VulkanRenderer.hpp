#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "../Renderer.hpp"

#include "Procedural/Commands/VulkanCommandBuffer.hpp"
#include "Procedural/Commands/VulkanCommandPool.hpp"
#include "Procedural/Core/VulkanInstance.hpp"
#include "Procedural/Core/VulkanLogicalDevice.hpp"
#include "Procedural/Core/VulkanPhysicalDevice.hpp"
#include "Procedural/Core/VulkanSurface.hpp"
#include "Procedural/Descriptors/VulkanDescriptorSetLayoutRegistry.hpp"
#include "Procedural/Pipeline/VulkanGraphicsPipeline.hpp"
#include "Procedural/Rendering/VulkanRenderPass.hpp"
#include "Procedural/Swapchain/VulkanSwapchain.hpp"
#include "Procedural/Sync/VulkanFence.hpp"
#include "Procedural/Sync/VulkanSemaphore.hpp"

namespace Engine::Graphics::Vulkan {

class ContextBackend;

class DescriptorSetLayoutRegistry;

struct FrameData {
    CommandBuffer commandBuffer { };

    Semaphore imageAvailableSemaphore { };
    Semaphore renderFinishedSemaphore { };
    Fence inFlightFence { };
};

class RendererBackend final : public Renderer {
public:
    RendererBackend(const ContextBackend &context);
    ~RendererBackend() override;

    void Init(Window::Window &window) override;
    void Destroy() override;

    bool BeginFrame(const Window::Window &window) override;
    void EndFrame(const Window::Window &window) override;

    void DrawMesh(
        const Allocator &allocator,
        const Assets::AssetRegistry &assets,
        MeshHandle mesh,
        uint32_t instanceCount,
        uint32_t firstIndex,
        int32_t vertexOffset,
        uint32_t firstInstance) override;

private:
    void TriggerSwapchainRecreation(const Window::Window &window);

    const ContextBackend &m_context;

    DescriptorSetLayoutRegistry m_descriptorSetLayoutRegistry;

    Swapchain m_swapchain;
    RenderPass m_renderPass;
    GraphicsPipeline m_graphicsPipeline;
    CommandPool m_commandPool;

    std::vector<FrameData> m_frames;
    uint32_t m_imageIndex = 0;
    uint32_t m_currentFrame = 0;
};

}
