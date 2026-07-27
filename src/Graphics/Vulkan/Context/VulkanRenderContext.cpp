#include "VulkanRenderContext.hpp"

#include <array>

#include "VulkanCoreContext.hpp"

#include "Graphics/Types/DescriptorTypes.hpp"
#include "Graphics/Types/ShaderStageTypes.hpp"

#include "Graphics/Vulkan/Helpers/VulkanVertexHelpers.hpp"
#include "Graphics/Vulkan/Procedural/Descriptors/VulkanDescriptorSetLayoutBindings.hpp"

namespace Engine::Graphics::Vulkan {

RenderContextBackend::RenderContextBackend(
    Passkey<RenderContextBackend>,
    const CoreContextBackend &coreContext,
    RenderPass renderPass,
    GraphicsPipeline graphicsPipeline,
    CommandPool commandPool,
    DescriptorPool descriptorPool,
    DescriptorWriter descriptorWriter,
    DescriptorSetLayoutRegistry descriptorSetLayoutRegistry,
    DescriptorSetLayoutHandle globalDescriptorSetLayout)
    : m_coreContext(coreContext)
    , m_renderPass(std::move(renderPass))
    , m_graphicsPipeline(std::move(graphicsPipeline))
    , m_commandPool(std::move(commandPool))
    , m_descriptorPool(std::move(descriptorPool))
    , m_descriptorWriter(std::move(descriptorWriter))
    , m_descriptorSetLayoutRegistry(std::move(descriptorSetLayoutRegistry))
    , m_globalDescriptorSetLayout(globalDescriptorSetLayout)
{
}

RenderContextBackend::~RenderContextBackend()
{
    Destroy();
}

std::unique_ptr<RenderContextBackend> RenderContextBackend::Create(const CoreContextBackend &coreContext)
{
    auto renderPass = Vulkan::CreateRenderPass(coreContext.GetLogicalDevice(), coreContext.GetSurface());

    auto commandPool = Vulkan::CreateCommandPool(
        coreContext.GetLogicalDevice(),
        coreContext.GetPhysicalDevice().queueFamilyIndices.graphicsFamily.value());

    auto descriptorPool = Vulkan::CreateDescriptorPool(
        coreContext.GetLogicalDevice(),
        { .maxSets = 2000, .uniformBufferCount = 1000, .imageSamplerCount = 1000 });

    DescriptorWriter descriptorWriter;
    DescriptorSetLayoutRegistry descriptorSetLayoutRegistry(coreContext.GetLogicalDevice());

    auto globalLayoutBindings = Vulkan::DescriptorSetLayoutBindings { };
    Vulkan::AddDescriptorLayoutBinding(
        globalLayoutBindings,
        DescriptorType::UNIFORM_BUFFER,
        0, 1,
        ShaderStage::VERTEX | ShaderStage::FRAGMENT); // Projection and View Matrices

    auto globalDescriptorSetLayout = descriptorSetLayoutRegistry.CreateDescriptorSetLayout(globalLayoutBindings);

    std::array<VkVertexInputBindingDescription, 1> bindingDescriptions = { Vulkan::GetVertexBindingDescription() };
    std::array<DescriptorSetLayoutHandle, 1> descriptorSetLayouts = { globalDescriptorSetLayout };

    auto attributeDescriptions = Vulkan::GetVertexAttributeDescriptions();

    auto graphicsPipeline = Vulkan::CreateGraphicsPipeline(
        coreContext.GetLogicalDevice(),
        renderPass,
        descriptorSetLayoutRegistry,
        bindingDescriptions,
        attributeDescriptions,
        descriptorSetLayouts);

    return std::make_unique<RenderContextBackend>(
        Passkey<RenderContextBackend> { },
        coreContext,
        std::move(renderPass),
        std::move(graphicsPipeline),
        std::move(commandPool),
        std::move(descriptorPool),
        std::move(descriptorWriter),
        std::move(descriptorSetLayoutRegistry),
        globalDescriptorSetLayout);
}

void RenderContextBackend::Destroy()
{
    WaitIdle(m_coreContext.GetLogicalDevice());

    Vulkan::DestroyGraphicsPipeline(m_coreContext.GetLogicalDevice().handle, m_graphicsPipeline);

    m_descriptorSetLayoutRegistry.Destroy();

    Vulkan::DestroyDescriptorPool(m_coreContext.GetLogicalDevice().handle, m_descriptorPool);
    Vulkan::DestroyCommandPool(m_coreContext.GetLogicalDevice(), m_commandPool);
    Vulkan::DestroyRenderPass(m_coreContext.GetLogicalDevice().handle, m_renderPass);
}

}
