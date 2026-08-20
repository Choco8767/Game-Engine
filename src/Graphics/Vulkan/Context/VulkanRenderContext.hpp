#pragma once

#include <memory>
#include <functional>

#include "Utils/Passkey.hpp"

#include "Graphics/Context/RenderContext.hpp"

#include "Graphics/Vulkan/Internal/Commands/VulkanCommandPool.hpp"
#include "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorPool.hpp"
#include "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorSetLayoutRegistry.hpp"
#include "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorWriter.hpp"
#include "Graphics/Vulkan/Internal/Pipeline/VulkanGraphicsPipeline.hpp"
#include "Graphics/Vulkan/Internal/Rendering/VulkanRenderPass.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;

class RenderContextBackend final : public Engine::Graphics::RenderContext {
public:
    RenderContextBackend(
        Passkey<RenderContextBackend>,
        const CoreContextBackend &coreContext,
        RenderPass renderPass,
        GraphicsPipeline graphicsPipeline,
        CommandPool commandPool,
        DescriptorPool descriptorPool,
        DescriptorWriter descriptorWriter,
        DescriptorSetLayoutRegistry descriptorSetLayoutRegistry,
        DescriptorSetLayoutHandle globalDescriptorSetLayout);
    ~RenderContextBackend() override;

    RenderContextBackend(const RenderContextBackend &other) = delete;
    RenderContextBackend &operator=(const RenderContextBackend &other) = delete;

    RenderContextBackend(RenderContextBackend &&other) noexcept = default;
    RenderContextBackend &operator=(RenderContextBackend &&other) noexcept = default;

    static std::unique_ptr<RenderContextBackend> Create(const CoreContextBackend &coreContext);
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
    std::reference_wrapper<const CoreContextBackend> m_coreContext;

    RenderPass m_renderPass;
    GraphicsPipeline m_graphicsPipeline;
    CommandPool m_commandPool;
    DescriptorPool m_descriptorPool;
    DescriptorWriter m_descriptorWriter;
    DescriptorSetLayoutRegistry m_descriptorSetLayoutRegistry;

    DescriptorSetLayoutHandle m_globalDescriptorSetLayout;
};

}
