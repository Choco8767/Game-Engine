#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "../RenderContext.hpp"

#include "Procedural/Commands/VulkanCommandPool.hpp"
#include "Procedural/Descriptors/VulkanDescriptorPool.hpp"
#include "Procedural/Descriptors/VulkanDescriptorSetLayoutRegistry.hpp"
#include "Procedural/Descriptors/VulkanDescriptorWriter.hpp"
#include "Procedural/Pipeline/VulkanGraphicsPipeline.hpp"
#include "Procedural/Rendering/VulkanRenderPass.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;

class RenderContextBackend final : public Engine::Graphics::RenderContext {
public:
    RenderContextBackend(const CoreContextBackend &coreContext);
    ~RenderContextBackend() override;

    void Init() override;
    void Destroy() override;

    // Getters
    API GetAPIType() const noexcept override { return API::VULKAN; }

    const RenderPass &GetRenderPass() const { return m_renderPass; }
    const GraphicsPipeline &GetGraphicsPipeline() const { return m_graphicsPipeline; }
    const CommandPool &GetCommandPool() const { return m_commandPool; }
    const DescriptorSetLayoutRegistry &GetDescriptorSetLayoutRegistry() const { return m_descriptorSetLayoutRegistry; }

    DescriptorPool &GetDescriptorPool() { return m_descriptorPool; }
    const DescriptorPool &GetDescriptorPool() const { return m_descriptorPool; }
    DescriptorWriter &GetDescriptorWriter() { return m_descriptorWriter; }
    const DescriptorWriter &GetDescriptorWriter() const { return m_descriptorWriter; }

    DescriptorSetLayoutHandle GetGlobalDescriptorSetLayout() const { return m_globalDescriptorSetLayout; }

private:
    const CoreContextBackend &m_coreContext;

    RenderPass m_renderPass;
    GraphicsPipeline m_graphicsPipeline;
    CommandPool m_commandPool;
    DescriptorPool m_descriptorPool;
    DescriptorWriter m_descriptorWriter;

    DescriptorSetLayoutHandle m_globalDescriptorSetLayout;

    DescriptorSetLayoutRegistry m_descriptorSetLayoutRegistry;
};

}
