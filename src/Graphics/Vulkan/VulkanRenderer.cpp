#include "VulkanRenderer.hpp"

#include <chrono>

#include "Window/Window.hpp"

#include "Assets/AssetRegistry.hpp"
#include "Assets/Types/GraphicsMesh.hpp"

#include "Graphics/Types/BufferTypes.hpp"
#include "Graphics/Types/DescriptorTypes.hpp"

#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"
#include "Graphics/Vulkan/Context/VulkanRenderContext.hpp"

#include "Graphics/Vulkan/Allocators/VulkanBufferAllocator.hpp"

#include "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorSet.hpp"
#include "Graphics/Vulkan/Internal/Resources/VulkanBuffer.hpp"

static auto start = std::chrono::high_resolution_clock::now();

namespace Engine::Graphics::Vulkan {

RendererBackend::RendererBackend(CoreContextBackend &coreContext, RenderContextBackend &renderContext, BufferAllocatorBackend &bufferAllocator)
    : m_coreContext(coreContext)
    , m_renderContext(renderContext)
    , m_bufferAllocator(bufferAllocator)
{
}

RendererBackend::~RendererBackend()
{
    Destroy();
}

void RendererBackend::Init(Engine::Window::Window &window)
{
    start = std::chrono::high_resolution_clock::now();

    m_swapchain = Vulkan::CreateSwapchain(window, m_coreContext.GetSurface(), m_coreContext.GetPhysicalDevice(), m_coreContext.GetLogicalDevice());
    Vulkan::InitSwapchainImageViews(m_coreContext.GetLogicalDevice(), m_swapchain);
    Vulkan::InitSwapchainFramebuffers(m_coreContext.GetLogicalDevice(), m_renderContext.GetRenderPass(), m_swapchain);

    m_frames.resize(m_swapchain.images.size());

    std::vector<DescriptorSetLayoutHandle> descriptorSetLayouts = {
        m_renderContext.GetGlobalDescriptorSetLayout(),
    };

    for (auto &frame : m_frames) {
        frame.commandBuffer = Vulkan::AllocateCommandBuffer(m_coreContext.GetLogicalDevice(), m_renderContext.GetCommandPool());
        frame.imageAvailableSemaphore = Vulkan::CreateSemaphore(m_coreContext.GetLogicalDevice());
        frame.renderFinishedSemaphore = Vulkan::CreateSemaphore(m_coreContext.GetLogicalDevice());
        frame.inFlightFence = Vulkan::CreateFence(m_coreContext.GetLogicalDevice(), true);

        frame.descriptorSets = Vulkan::AllocateDescriptorSets(m_coreContext.GetLogicalDevice(), m_renderContext.GetDescriptorSetLayoutRegistry(), descriptorSetLayouts, m_renderContext.GetDescriptorPool());

        frame.uniformBuffer = m_bufferAllocator.CreateBuffer({
            .size = sizeof(UniformBufferData),
            .usage = BufferUsage::UNIFORM,
        });

        m_renderContext.GetDescriptorWriter()
            .WriteBuffer(m_bufferAllocator.GetBuffer(frame.uniformBuffer), 0, DescriptorType::UNIFORM_BUFFER, 0, 0);

        for (auto descriptorSet : frame.descriptorSets)
            m_renderContext.GetDescriptorWriter().Update(m_coreContext.GetLogicalDevice(), descriptorSet);
    }
}

void RendererBackend::Destroy()
{
    WaitIdle(m_coreContext.GetLogicalDevice());

    Vulkan::DestroySwapchain(m_coreContext.GetLogicalDevice().handle, m_swapchain);

    for (auto &frame : m_frames) {
        FreeCommandBuffer(m_coreContext.GetLogicalDevice().handle, frame.commandBuffer, m_renderContext.GetCommandPool());
        DestroySemaphore(m_coreContext.GetLogicalDevice().handle, frame.imageAvailableSemaphore);
        DestroySemaphore(m_coreContext.GetLogicalDevice().handle, frame.renderFinishedSemaphore);
        DestroyFence(m_coreContext.GetLogicalDevice().handle, frame.inFlightFence);
    }
}

void RendererBackend::TriggerSwapchainRecreation(const Engine::Window::Window &window)
{
    Vulkan::RecreateSwapchain(
        window,
        m_coreContext.GetSurface(),
        m_coreContext.GetPhysicalDevice(),
        m_coreContext.GetLogicalDevice(),
        m_renderContext.GetRenderPass(),
        m_swapchain);
}

bool RendererBackend::BeginFrame(const Engine::Window::Window &window)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    Vulkan::WaitForFence(m_coreContext.GetLogicalDevice(), currentFrame.inFlightFence);

    if (window.HasResized()) {
        TriggerSwapchainRecreation(window);
        return false;
    }

    auto acquireImageResult = Vulkan::AcquireNextSwapchainImage(m_coreContext.GetLogicalDevice(), currentFrame.imageAvailableSemaphore, m_swapchain);
    if (acquireImageResult.NeedsRecreation()) {
        TriggerSwapchainRecreation(window);
        return false;
    }

    m_imageIndex = acquireImageResult.imageIndex;

    Vulkan::ResetFence(m_coreContext.GetLogicalDevice(), currentFrame.inFlightFence);

    Vulkan::BeginCommandBuffer(currentFrame.commandBuffer);
    Vulkan::BeginRenderPass(
        currentFrame.commandBuffer,
        m_renderContext.GetRenderPass(),
        m_swapchain.framebuffers[m_imageIndex],
        m_swapchain.extent);
    Vulkan::BindPipeline(
        currentFrame.commandBuffer,
        m_renderContext.GetGraphicsPipeline(),
        m_swapchain.extent);

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - start;

    double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed).count();

    m_uniformBufferData.time = static_cast<float>(seconds);

    m_bufferAllocator.UpdateBuffer(currentFrame.uniformBuffer, &m_uniformBufferData, sizeof(m_uniformBufferData), 0);
    Vulkan::BindDescriptorSets(currentFrame.commandBuffer, m_renderContext.GetGraphicsPipeline(), currentFrame.descriptorSets);

    return true;
}

void RendererBackend::EndFrame(const Engine::Window::Window &window)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    Vulkan::EndRenderPass(currentFrame.commandBuffer);
    Vulkan::EndCommandBuffer(currentFrame.commandBuffer);

    Vulkan::SubmitCommandBuffer(
        m_coreContext.GetLogicalDevice().graphicsQueue,
        currentFrame.commandBuffer,
        currentFrame.imageAvailableSemaphore,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        currentFrame.renderFinishedSemaphore,
        currentFrame.inFlightFence);

    auto presentImageResult = Vulkan::PresentSwapchainImage(
        m_coreContext.GetLogicalDevice().presentQueue,
        m_swapchain,
        m_imageIndex,
        currentFrame.renderFinishedSemaphore);

    if (presentImageResult.NeedsRecreation()) {
        TriggerSwapchainRecreation(window);
        return;
    }

    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}

void RendererBackend::DrawMesh(
    const Assets::AssetRegistry &assets,
    MeshHandle mesh,
    std::uint32_t instanceCount,
    std::uint32_t firstIndex,
    std::int32_t vertexOffset,
    std::uint32_t firstInstance)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    const auto &rawMesh = assets.GetMesh(mesh);

    Vulkan::BindVertexBuffer(currentFrame.commandBuffer, m_bufferAllocator.GetBuffer(rawMesh.vertexBuffer), 0, 1);
    Vulkan::BindIndexBuffer(currentFrame.commandBuffer, m_bufferAllocator.GetBuffer(rawMesh.indexBuffer));
    Vulkan::DrawIndexed(currentFrame.commandBuffer, rawMesh.indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

}
