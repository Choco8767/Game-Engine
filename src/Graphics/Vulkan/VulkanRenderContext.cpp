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
{
}

RenderContextBackend::~RenderContextBackend()
{
    Destroy();
}

void RenderContextBackend::Init()
{
    m_renderPass = Vulkan::CreateRenderPass(m_coreContext.GetLogicalDevice(), m_coreContext.GetSurface());

    auto bindingDescription = Vulkan::GetVertexBindingDescription();
    auto attributeDescriptions = Vulkan::GetVertexAttributeDescriptions();

    auto layoutBindings = Vulkan::DescriptorSetLayoutBindings { };
    // Vulkan::AddDescriptorLayoutBinding(layoutBindings, DescriptorType::UNIFORM_BUFFER, 0, 1, ShaderStage::VERTEX);

    auto descriptorSetLayout = m_descriptorSetLayoutRegistry.CreateDescriptorSetLayout(layoutBindings);

    m_graphicsPipeline = Vulkan::CreateGraphicsPipeline(
        m_coreContext.GetLogicalDevice(),
        m_renderPass,
        m_descriptorSetLayoutRegistry,
        std::span<VkVertexInputBindingDescription>(&bindingDescription, 1),
        attributeDescriptions,
        std::span<DescriptorSetLayoutHandle>(&descriptorSetLayout, 1));

    m_commandPool = Vulkan::CreateCommandPool(
        m_coreContext.GetLogicalDevice(),
        m_coreContext.GetPhysicalDevice().queueFamilyIndices.graphicsFamily.value());
}

void RenderContextBackend::Destroy()
{
    WaitIdle(m_coreContext.GetLogicalDevice());

    Vulkan::DestroyCommandPool(m_coreContext.GetLogicalDevice().handle, m_commandPool);
    Vulkan::DestroyGraphicsPipeline(m_coreContext.GetLogicalDevice().handle, m_graphicsPipeline);
    Vulkan::DestroyRenderPass(m_coreContext.GetLogicalDevice().handle, m_renderPass);
}

}
