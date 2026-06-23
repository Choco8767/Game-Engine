#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "../Renderer.hpp"

#include "Procedural/Commands/VulkanCommandBuffer.hpp"
#include "Procedural/Swapchain/VulkanSwapchain.hpp"
#include "Procedural/Sync/VulkanFence.hpp"
#include "Procedural/Sync/VulkanSemaphore.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;
class RenderContextBackend;

struct FrameData {
    CommandBuffer commandBuffer { };

    Semaphore imageAvailableSemaphore { };
    Semaphore renderFinishedSemaphore { };
    Fence inFlightFence { };
};

class RendererBackend final : public Renderer {
public:
    RendererBackend(const CoreContextBackend &coreContext, const RenderContextBackend &renderContext);
    ~RendererBackend() override;

    void Init(Window::Window &window) override;
    void Destroy() override;

    void TriggerSwapchainRecreation(const Engine::Window::Window &window);

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
    const CoreContextBackend &m_coreContext;
    const RenderContextBackend &m_renderContext;

    Swapchain m_swapchain;

    std::vector<FrameData> m_frames;
    uint32_t m_imageIndex = 0;
    uint32_t m_currentFrame = 0;
};

}
