#include "VulkanRenderer.hpp"

#include "Window/Window.hpp"

#include <optional>

namespace Engine::Vulkan {

RendererBackend::~RendererBackend()
{
    Destroy();
}

void RendererBackend::Init(Window &window)
{
    auto requiredExtensions = window.GetRequiredInstanceExtensions();

    std::optional<Instance> optInstance = Vulkan::CreateInstance(requiredExtensions);
    if (optInstance.has_value())
        m_instance = optInstance.value();

    std::optional<Surface> optSurface = Vulkan::CreateSurface(m_instance, window);
    if (optSurface.has_value())
        m_surface = optSurface.value();

    std::optional<PhysicalDevice> optPhysicalDevice = Vulkan::CreatePhysicalDevice(m_instance, m_surface);
    if (optPhysicalDevice.has_value())
        m_physicalDevice = optPhysicalDevice.value();

    std::optional<LogicalDevice> optLogicalDevice = Vulkan::CreateLogicalDevice(m_physicalDevice);
    if (optLogicalDevice.has_value())
        m_logicalDevice = optLogicalDevice.value();

    std::optional<Swapchain> optSwapchain = Vulkan::CreateSwapchain(window, m_surface, m_physicalDevice, m_logicalDevice);
    if (optSwapchain.has_value())
        m_swapchain = optSwapchain.value();
    Vulkan::InitSwapchainImageViews(m_logicalDevice, m_swapchain);

    std::optional<RenderPass> optRenderPass = Vulkan::CreateRenderPass(m_logicalDevice, m_swapchain);
    if (optRenderPass.has_value())
        m_renderPass = optRenderPass.value();
    Vulkan::InitSwapchainFramebuffers(m_logicalDevice, m_renderPass, m_swapchain);

    std::optional<GraphicsPipeline> optGraphicsPipeline = Vulkan::CreateGraphicsPipeline(m_logicalDevice, m_renderPass);
    if (optGraphicsPipeline.has_value())
        m_graphicsPipeline = optGraphicsPipeline.value();

    std::optional<CommandPool> optCommandPool = Vulkan::CreateCommandPool(m_physicalDevice, m_logicalDevice);
    if (optCommandPool.has_value())
        m_commandPool = optCommandPool.value();

    m_frames.resize(m_swapchain.images.size());
    for (auto &frame : m_frames) {
        std::optional<CommandBuffer> optCommandBuffer = Vulkan::AllocateCommandBuffer(m_logicalDevice, m_commandPool);
        if (optCommandBuffer.has_value())
            frame.commandBuffer = optCommandBuffer.value();

        std::optional<Semaphore> optImageAvailableSemaphore = Vulkan::CreateSemaphore(m_logicalDevice);
        if (optImageAvailableSemaphore.has_value())
            frame.imageAvailableSemaphore = optImageAvailableSemaphore.value();

        std::optional<Semaphore> optRenderFinishedSemaphore = Vulkan::CreateSemaphore(m_logicalDevice);
        if (optRenderFinishedSemaphore.has_value())
            frame.renderFinishedSemaphore = optRenderFinishedSemaphore.value();

        std::optional<Fence> optinFlightFence = Vulkan::CreateFence(m_logicalDevice, true);
        if (optinFlightFence.has_value())
            frame.inFlightFence = optinFlightFence.value();
    }
}

void RendererBackend::Draw()
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    Vulkan::WaitForFence(m_logicalDevice, currentFrame.inFlightFence);
    Vulkan::ResetFence(m_logicalDevice, currentFrame.inFlightFence);

    uint32_t swapchainImage = Vulkan::AquireNextSwapchainImage(m_logicalDevice, currentFrame.imageAvailableSemaphore, m_swapchain);

    Vulkan::BeginCommandBuffer(currentFrame.commandBuffer);
    Vulkan::BeginRenderPass(
        currentFrame.commandBuffer,
        m_renderPass,
        m_swapchain.framebuffers[m_currentFrame],
        m_swapchain.extent);
    Vulkan::BindPipeline(
        currentFrame.commandBuffer,
        m_graphicsPipeline,
        m_swapchain.extent);
    Vulkan::Draw(currentFrame.commandBuffer, 3, 1, 0, 0);
    Vulkan::EndRenderPass(currentFrame.commandBuffer);
    Vulkan::EndCommandBuffer(currentFrame.commandBuffer);

    Vulkan::SubmitCommandBuffer(
        m_logicalDevice.graphicsQueue,
        currentFrame.commandBuffer,
        currentFrame.imageAvailableSemaphore,
        currentFrame.renderFinishedSemaphore,
        currentFrame.inFlightFence);

    Vulkan::PresentSwapchainImage(
        m_logicalDevice.presentQueue,
        m_swapchain,
        swapchainImage,
        currentFrame.renderFinishedSemaphore);

    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}

void RendererBackend::Destroy()
{
    WaitIdle(m_logicalDevice);

    for (auto &frame : m_frames) {
        FreeCommandBuffer(m_logicalDevice.handle, frame.commandBuffer, m_commandPool);
        DestroySemaphore(m_logicalDevice.handle, frame.imageAvailableSemaphore);
        DestroySemaphore(m_logicalDevice.handle, frame.renderFinishedSemaphore);
        DestroyFence(m_logicalDevice.handle, frame.inFlightFence);
    }

    Vulkan::DestroyCommandPool(m_logicalDevice.handle, m_commandPool);
    Vulkan::DestroyGraphicsPipeline(m_logicalDevice.handle, m_graphicsPipeline);
    Vulkan::DestroyRenderPass(m_logicalDevice.handle, m_renderPass);
    Vulkan::DestroySwapchain(m_logicalDevice.handle, m_swapchain);
    Vulkan::DestroyLogicalDevice(m_logicalDevice);
    Vulkan::DestroyPhysicalDevice(m_physicalDevice);
    Vulkan::DestroySurface(m_instance.handle, m_surface);
    Vulkan::DestroyInstance(m_instance);
}

}
