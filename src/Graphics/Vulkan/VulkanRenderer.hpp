#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "Assets/Types/AssetHandles.hpp"
#include "Graphics/Types/GraphicsHandles.hpp"

#include "Procedural/Commands/VulkanCommandBuffer.hpp"
#include "Procedural/Swapchain/VulkanSwapchain.hpp"
#include "Procedural/Sync/VulkanFence.hpp"
#include "Procedural/Sync/VulkanSemaphore.hpp"

namespace Engine::Assets {

class AssetRegistry;

}
namespace Engine::Graphics {

class Allocator;

}

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;
class RenderContextBackend;
class AllocatorBackend;

struct FrameData {
    CommandBuffer commandBuffer { };

    Semaphore imageAvailableSemaphore { };
    Semaphore renderFinishedSemaphore { };
    Fence inFlightFence { };

    std::vector<DescriptorSet> descriptorSets;

    BufferHandle uniformBuffer { };
};

struct UniformBufferData {
    float time = 0.0f;
};

class RendererBackend final {
public:
    RendererBackend(CoreContextBackend &coreContext, RenderContextBackend &renderContext, AllocatorBackend &allocator);
    ~RendererBackend();

    void Init(Window::Window &window);
    void Destroy();

    void TriggerSwapchainRecreation(const Engine::Window::Window &window);

    bool BeginFrame(const Window::Window &window);
    void EndFrame(const Window::Window &window);

    void DrawMesh(
        const Graphics::Allocator &allocator,
        const Assets::AssetRegistry &assets,
        MeshHandle mesh,
        uint32_t instanceCount = 1,
        uint32_t firstIndex = 0,
        int32_t vertexOffset = 0,
        uint32_t firstInstance = 0);

private:
    CoreContextBackend &m_coreContext;
    RenderContextBackend &m_renderContext;
    AllocatorBackend &m_allocator;

    Swapchain m_swapchain;

    UniformBufferData m_uniformBufferData { };

    std::vector<FrameData> m_frames;
    uint32_t m_imageIndex = 0;
    uint32_t m_currentFrame = 0;
};

}
