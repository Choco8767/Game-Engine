#include "VulkanRenderContext.hpp"

#include "VulkanCoreContext.hpp"

#include "Graphics/Types/DescriptorTypes.hpp"
#include "Graphics/Types/ShaderStageTypes.hpp"
#include "Helpers/VulkanVertexHelpers.hpp"
#include "Procedural/Descriptors/VulkanDescriptorSetLayoutBindings.hpp"

namespace Engine::Graphics::Vulkan {

RenderContextBackend::RenderContextBackend(const CoreContextBackend &coreContext)
    : m_coreContext(coreContext)
    , m_descriptorSetLayoutRegistry(m_coreContext.GetLogicalDevice())
    , m_descriptorWriter()
{
}

RenderContextBackend::~RenderContextBackend()
{
    Destroy();
}

void RenderContextBackend::Init()
{
    m_renderPass = Vulkan::CreateRenderPass(m_coreContext.GetLogicalDevice(), m_coreContext.GetSurface());

    m_commandPool = Vulkan::CreateCommandPool(
        m_coreContext.GetLogicalDevice(),
        m_coreContext.GetPhysicalDevice().queueFamilyIndices.graphicsFamily.value());

    m_descriptorPool = Vulkan::CreateDescriptorPool(
        m_coreContext.GetLogicalDevice(),
        { .maxSets = 2000, .uniformBufferCount = 1000, .imageSamplerCount = 1000 });

    auto bindingDescription = Vulkan::GetVertexBindingDescription();
    auto attributeDescriptions = Vulkan::GetVertexAttributeDescriptions();

    auto globalLayoutBindings = Vulkan::DescriptorSetLayoutBindings { };
    Vulkan::AddDescriptorLayoutBinding(
        globalLayoutBindings,
        DescriptorType::UNIFORM_BUFFER,
        0, 1,
        ShaderStage::VERTEX | ShaderStage::FRAGMENT); // Projection and View Matrices

    m_globalDescriptorSetLayout = m_descriptorSetLayoutRegistry.CreateDescriptorSetLayout(globalLayoutBindings);

    m_graphicsPipeline = Vulkan::CreateGraphicsPipeline(
        m_coreContext.GetLogicalDevice(),
        m_renderPass,
        m_descriptorSetLayoutRegistry,
        std::span<VkVertexInputBindingDescription>(&bindingDescription, 1),
        attributeDescriptions,
        std::span<DescriptorSetLayoutHandle>(&m_globalDescriptorSetLayout, 1));
}

void RenderContextBackend::Destroy()
{
    WaitIdle(m_coreContext.GetLogicalDevice());

    Vulkan::DestroyDescriptorPool(m_coreContext.GetLogicalDevice().handle, m_descriptorPool);
    Vulkan::DestroyCommandPool(m_coreContext.GetLogicalDevice().handle, m_commandPool);
    Vulkan::DestroyGraphicsPipeline(m_coreContext.GetLogicalDevice().handle, m_graphicsPipeline);
    Vulkan::DestroyRenderPass(m_coreContext.GetLogicalDevice().handle, m_renderPass);
}

}
