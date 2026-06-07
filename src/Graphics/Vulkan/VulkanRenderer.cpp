#include "VulkanRenderer.hpp"

#include "Assets/AssetRegistry.hpp"
#include "Assets/Types/GraphicsMesh.hpp"

#include "VulkanAllocator.hpp"
#include "VulkanContext.hpp"
#include "Window/Window.hpp"

#include "Helpers/VulkanVertexHelpers.hpp"

namespace Engine::Graphics::Vulkan {

RendererBackend::RendererBackend(const ContextBackend &context)
    : m_context(context)
{
}

RendererBackend::~RendererBackend()
{
    Destroy();
}

void RendererBackend::Init(Engine::Window::Window &window)
{
    m_swapchain = Vulkan::CreateSwapchain(window, m_context.GetSurface(), m_context.GetPhysicalDevice(), m_context.GetLogicalDevice());
    Vulkan::InitSwapchainImageViews(m_context.GetLogicalDevice(), m_swapchain);

    m_renderPass = Vulkan::CreateRenderPass(m_context.GetLogicalDevice(), m_swapchain);
    Vulkan::InitSwapchainFramebuffers(m_context.GetLogicalDevice(), m_renderPass, m_swapchain);

    auto bindingDescription = Vulkan::GetVertexBindingDescription();
    auto attributeDescriptions = Vulkan::GetVertexAttributeDescriptions();

    m_graphicsPipeline = Vulkan::CreateGraphicsPipeline(
        m_context.GetLogicalDevice(),
        m_renderPass,
        std::span<VkVertexInputBindingDescription>(&bindingDescription, 1),
        attributeDescriptions);
    m_commandPool = Vulkan::CreateCommandPool(
        m_context.GetLogicalDevice(),
        m_context.GetPhysicalDevice().queueFamilyIndices.graphicsFamily.value());

    m_frames.resize(m_swapchain.images.size());

    for (auto &frame : m_frames) {
        frame.commandBuffer = Vulkan::AllocateCommandBuffer(m_context.GetLogicalDevice(), m_commandPool);
        frame.imageAvailableSemaphore = Vulkan::CreateSemaphore(m_context.GetLogicalDevice());
        frame.renderFinishedSemaphore = Vulkan::CreateSemaphore(m_context.GetLogicalDevice());
        frame.inFlightFence = Vulkan::CreateFence(m_context.GetLogicalDevice(), true);
    }
}

void RendererBackend::TriggerSwapchainRecreation(const Engine::Window::Window &window)
{
    Vulkan::RecreateSwapchain(
        window,
        m_context.GetSurface(),
        m_context.GetPhysicalDevice(),
        m_context.GetLogicalDevice(),
        m_renderPass,
        m_swapchain);
}

bool RendererBackend::BeginFrame(const Engine::Window::Window &window)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    Vulkan::WaitForFence(m_context.GetLogicalDevice(), currentFrame.inFlightFence);

    if (window.HasResized()) {
        TriggerSwapchainRecreation(window);
        return false;
    }

    auto acquireImageResult = Vulkan::AcquireNextSwapchainImage(m_context.GetLogicalDevice(), currentFrame.imageAvailableSemaphore, m_swapchain);
    if (acquireImageResult.NeedsRecreation()) {
        TriggerSwapchainRecreation(window);
        return false;
    }

    m_imageIndex = acquireImageResult.imageIndex;

    Vulkan::ResetFence(m_context.GetLogicalDevice(), currentFrame.inFlightFence);

    Vulkan::BeginCommandBuffer(currentFrame.commandBuffer);
    Vulkan::BeginRenderPass(
        currentFrame.commandBuffer,
        m_renderPass,
        m_swapchain.framebuffers[m_imageIndex],
        m_swapchain.extent);
    Vulkan::BindPipeline(
        currentFrame.commandBuffer,
        m_graphicsPipeline,
        m_swapchain.extent);

    return true;
}

void RendererBackend::EndFrame(const Engine::Window::Window &window)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    Vulkan::EndRenderPass(currentFrame.commandBuffer);
    Vulkan::EndCommandBuffer(currentFrame.commandBuffer);

    Vulkan::SubmitCommandBuffer(
        m_context.GetLogicalDevice().graphicsQueue,
        currentFrame.commandBuffer,
        currentFrame.imageAvailableSemaphore,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        currentFrame.renderFinishedSemaphore,
        currentFrame.inFlightFence);

    auto presentImageResult = Vulkan::PresentSwapchainImage(
        m_context.GetLogicalDevice().presentQueue,
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
    const Allocator &allocator,
    const Assets::AssetRegistry &assets,
    MeshHandle mesh,
    uint32_t instanceCount,
    uint32_t firstIndex,
    int32_t vertexOffset,
    uint32_t firstInstance)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    const auto &vulkanAllocator = static_cast<const AllocatorBackend &>(allocator);

    const auto &rawMesh = assets.GetMesh(mesh);

    Vulkan::BindVertexBuffer(currentFrame.commandBuffer, vulkanAllocator, rawMesh.vertexBuffer, 0, 1);
    Vulkan::BindIndexBuffer(currentFrame.commandBuffer, vulkanAllocator, rawMesh.indexBuffer);
    Vulkan::DrawIndexed(currentFrame.commandBuffer, rawMesh.indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void RendererBackend::Destroy()
{
    WaitIdle(m_context.GetLogicalDevice());

    for (auto &frame : m_frames) {
        FreeCommandBuffer(m_context.GetLogicalDevice().handle, frame.commandBuffer, m_commandPool);
        DestroySemaphore(m_context.GetLogicalDevice().handle, frame.imageAvailableSemaphore);
        DestroySemaphore(m_context.GetLogicalDevice().handle, frame.renderFinishedSemaphore);
        DestroyFence(m_context.GetLogicalDevice().handle, frame.inFlightFence);
    }

    Vulkan::DestroyCommandPool(m_context.GetLogicalDevice().handle, m_commandPool);
    Vulkan::DestroyGraphicsPipeline(m_context.GetLogicalDevice().handle, m_graphicsPipeline);
    Vulkan::DestroyRenderPass(m_context.GetLogicalDevice().handle, m_renderPass);
    Vulkan::DestroySwapchain(m_context.GetLogicalDevice().handle, m_swapchain);
}

}
