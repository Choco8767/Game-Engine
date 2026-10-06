#include "VulkanRenderer.hpp"

#include <cassert>
#include <chrono>
#include <format>
#include <memory>
#include <stdexcept>

#include "Window/Window.hpp"

#include "Graphics/Types/GraphicsHandles.hpp"

#include "Graphics/Vulkan/Context/VulkanCoreContext.hpp"
#include "Graphics/Vulkan/Context/VulkanRenderContext.hpp"
#include "Graphics/Vulkan/Context/VulkanAllocatorContext.hpp"

#include "Graphics/Vulkan/Allocators/VulkanBufferAllocator.hpp"

#include "Graphics/Vulkan/Internal/Resources/VulkanBuffer.hpp"
#include "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorSet.hpp"

namespace Engine::Graphics::Vulkan {

namespace {

constexpr std::size_t FRAMES_IN_FLIGHT = 2;

}

RendererBackend::RendererBackend(
    Passkey<RendererBackend>,
    CoreContextBackend &coreContext,
    RenderContextBackend &renderContext,
    AllocatorContextBackend &allocatorContext,
    Swapchain swapchain,
    std::vector<FrameData> frames,
    std::vector<Semaphore> renderFinishedSemaphores)
    : m_renderFinishedSemaphores(std::move(renderFinishedSemaphores))
    , m_coreContext(coreContext)
    , m_renderContext(renderContext)
    , m_allocatorContext(allocatorContext)
    , m_swapchain(std::move(swapchain))
    , m_frames(std::move(frames))
{
}

RendererBackend::~RendererBackend()
{
    Destroy();
}

std::unique_ptr<RendererBackend> RendererBackend::Create(
    const Engine::Window::Window &window,
    CoreContextBackend &coreContext,
    RenderContextBackend &renderContext,
    AllocatorContextBackend &allocatorContext)
{
    auto swapchain = Vulkan::CreateSwapchain(window, coreContext.GetSurface(), coreContext.GetPhysicalDevice(), coreContext.GetLogicalDevice());
    Vulkan::InitSwapchainImageViews(coreContext.GetLogicalDevice(), swapchain);
    Vulkan::InitSwapchainFramebuffers(coreContext.GetLogicalDevice(), renderContext.GetRenderPass(), swapchain);

    std::vector<DescriptorSetLayoutHandle> descriptorSetLayouts = {
        renderContext.GetGlobalDescriptorSetLayout(),
    };

    std::vector<FrameData> frames(FRAMES_IN_FLIGHT);

    for (auto &frame : frames) {
        frame.commandBuffer = Vulkan::AllocateCommandBuffer(coreContext.GetLogicalDevice(), renderContext.GetCommandPool());
        frame.imageAvailableSemaphore = Vulkan::CreateSemaphore(coreContext.GetLogicalDevice());
        frame.inFlightFence = Vulkan::CreateFence(coreContext.GetLogicalDevice(), true);

        frame.descriptorSets = Vulkan::AllocateDescriptorSets(coreContext.GetLogicalDevice(), renderContext.GetDescriptorSetLayoutRegistry(), descriptorSetLayouts, renderContext.GetDescriptorPool());

        frame.uniformBuffer = allocatorContext.GetBufferAllocator().CreateBuffer({
            .size = sizeof(UniformBufferData),
            .usage = BufferUsage::UNIFORM,
        });

        renderContext.GetDescriptorWriter()
            .WriteBuffer(allocatorContext.GetBufferAllocator().GetBuffer(frame.uniformBuffer), 0, DescriptorType::UNIFORM_BUFFER, 0, 0);

        for (auto descriptorSet : frame.descriptorSets)
            renderContext.GetDescriptorWriter().Update(coreContext.GetLogicalDevice(), descriptorSet);
    }

    std::vector<Semaphore> renderFinishedSemaphores;
    renderFinishedSemaphores.reserve(swapchain.images.size());
    for (std::size_t i = 0; i < swapchain.images.size(); i++)
        renderFinishedSemaphores.push_back(Vulkan::CreateSemaphore(coreContext.GetLogicalDevice()));

    return std::make_unique<RendererBackend>(
        Passkey<RendererBackend>{},
        coreContext,
        renderContext,
        allocatorContext,
        std::move(swapchain),
        std::move(frames),
        std::move(renderFinishedSemaphores)
    );
}

void RendererBackend::Destroy()
{
    WaitIdle(m_coreContext.GetLogicalDevice());

    Vulkan::DestroySwapchain(m_coreContext.GetLogicalDevice().handle, m_swapchain);

    for (auto &semaphore : m_renderFinishedSemaphores) {
        Vulkan::DestroySemaphore(m_coreContext.GetLogicalDevice().handle, semaphore);
    }

    for (auto &frame : m_frames) {
        FreeCommandBuffer(m_coreContext.GetLogicalDevice().handle, frame.commandBuffer, m_renderContext.GetCommandPool());
        DestroySemaphore(m_coreContext.GetLogicalDevice().handle, frame.imageAvailableSemaphore);
        DestroyFence(m_coreContext.GetLogicalDevice().handle, frame.inFlightFence);
    }
}

void RendererBackend::SyncRenderFinishedSemaphores()
{
    const std::size_t required = m_swapchain.images.size();

    while (m_renderFinishedSemaphores.size() > required) {
        Vulkan::DestroySemaphore(m_coreContext.GetLogicalDevice().handle, m_renderFinishedSemaphores.back());
        m_renderFinishedSemaphores.pop_back();
    }

    while (m_renderFinishedSemaphores.size() < required)
        m_renderFinishedSemaphores.push_back(Vulkan::CreateSemaphore(m_coreContext.GetLogicalDevice()));

    if (m_renderFinishedSemaphores.size() == m_swapchain.images.size())
        throw std::runtime_error("Render Finished Semaphores must track the Swapchain Image Count");
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

    SyncRenderFinishedSemaphores();
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

    if (!acquireImageResult.Succeeded())
        throw std::runtime_error(std::format("Failed to Acquire Next Swapchain Image. Error Code: {}", static_cast<int>(acquireImageResult.result)));

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
    auto elapsed = now - m_start;

    double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed).count();

    m_uniformBufferData.time = static_cast<float>(seconds);

    m_allocatorContext.GetBufferAllocator().UpdateBuffer(currentFrame.uniformBuffer, &m_uniformBufferData, sizeof(m_uniformBufferData), 0);
    Vulkan::BindDescriptorSets(currentFrame.commandBuffer, m_renderContext.GetGraphicsPipeline(), currentFrame.descriptorSets);

    return true;
}

void RendererBackend::EndFrame(const Engine::Window::Window &window)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    assert(m_imageIndex < m_renderFinishedSemaphores.size()
        && "m_imageIndex out of range: a swapchain recreation did not resync the semaphores");

    const Semaphore renderFinished = m_renderFinishedSemaphores[m_imageIndex];

    Vulkan::EndRenderPass(currentFrame.commandBuffer);
    Vulkan::EndCommandBuffer(currentFrame.commandBuffer);

    Vulkan::SubmitCommandBuffer(
        m_coreContext.GetLogicalDevice().graphicsQueue,
        currentFrame.commandBuffer,
        currentFrame.imageAvailableSemaphore,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        renderFinished,
        currentFrame.inFlightFence);

    auto presentImageResult = Vulkan::PresentSwapchainImage(
        m_coreContext.GetLogicalDevice().presentQueue,
        m_swapchain,
        m_imageIndex,
        renderFinished);

    if (presentImageResult.NeedsRecreation()) {
        TriggerSwapchainRecreation(window);
        return;
    }

    if (!presentImageResult.Succeeded())
        throw std::runtime_error(std::format("Failed to Present Swapchain Image. Error Code: {}", static_cast<int>(presentImageResult.result)));

    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}

void RendererBackend::DrawMeshes(std::span<const Assets::MeshAsset> meshes)
{
    FrameData &currentFrame = m_frames[m_currentFrame];

    for (const auto &mesh : meshes) {
        Vulkan::BindVertexBuffer(currentFrame.commandBuffer, m_allocatorContext.GetBufferAllocator().GetBuffer(mesh.vertexBuffer), 0, 1);
        Vulkan::BindIndexBuffer(currentFrame.commandBuffer, m_allocatorContext.GetBufferAllocator().GetBuffer(mesh.indexBuffer));
        Vulkan::DrawIndexed(currentFrame.commandBuffer, static_cast<std::uint32_t>(mesh.indexCount), 1, 0, 0, 0);
    }
}

}
