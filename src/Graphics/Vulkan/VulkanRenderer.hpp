#pragma once

#include <chrono>
#include <memory>
#include <span>
#include <vector>

#include <volk.h>

#include "Utils/Passkey.hpp"

#include "Assets/Types/MeshAsset.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Types/GraphicsHandles.hpp"

#include "Internal/Commands/VulkanCommandBuffer.hpp"
#include "Internal/Swapchain/VulkanSwapchain.hpp"
#include "Internal/Sync/VulkanFence.hpp"
#include "Internal/Sync/VulkanSemaphore.hpp"

namespace Engine::Window {

class Window;

}

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;
class RenderContextBackend;
class AllocatorContextBackend;

struct FrameData {
    CommandBuffer commandBuffer {};

    Semaphore imageAvailableSemaphore {};
    Fence inFlightFence {};

    std::vector<DescriptorSet> descriptorSets;

    BufferHandle uniformBuffer {};
};

struct UniformBufferData {
    float time = 0.0f;
};

class RendererBackend final : public Engine::Graphics::Renderer {
public:
    RendererBackend(
        Passkey<RendererBackend>,
        CoreContextBackend &coreContext,
        RenderContextBackend &renderContext,
        AllocatorContextBackend &allocatorContext,
        Swapchain swapchain,
        std::vector<FrameData> frames,
        std::vector<Semaphore> renderFinishedSemaphores);
    ~RendererBackend() override;

    RendererBackend(const RendererBackend &other) = delete;
    RendererBackend &operator=(const RendererBackend &other) = delete;

    RendererBackend(RendererBackend &&other) noexcept = default;
    RendererBackend &operator=(RendererBackend &&other) noexcept = delete;

    static std::unique_ptr<RendererBackend> Create(
        const Engine::Window::Window &window,
        CoreContextBackend &coreContext,
        RenderContextBackend &renderContext,
        AllocatorContextBackend &allocatorContext);
    void Destroy() override;

    // Getters
    API GetAPIType() const noexcept override { return API::VULKAN; }

    void TriggerSwapchainRecreation(const Engine::Window::Window &window);

    bool BeginFrame(const Window::Window &window) override;
    void EndFrame(const Window::Window &window) override;

    void DrawMeshes(std::span<const Assets::MeshAsset> meshes) override;

private:
    std::vector<Semaphore> m_renderFinishedSemaphores;

    void SyncRenderFinishedSemaphores();

    CoreContextBackend &m_coreContext;
    RenderContextBackend &m_renderContext;
    AllocatorContextBackend &m_allocatorContext;

    Swapchain m_swapchain;

    UniformBufferData m_uniformBufferData {};

    std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now();

    std::vector<FrameData> m_frames;
    std::uint32_t m_imageIndex = 0;
    std::uint32_t m_currentFrame = 0;
};

}
