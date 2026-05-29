#include "VulkanGraphicsPipeline.hpp"

#include <iostream>

#include "../Core/VulkanLogicalDevice.hpp"
#include "../Rendering/VulkanRenderPass.hpp"

#include "Utils/ReadFile.hpp"

namespace Engine::Vulkan {

std::optional<GraphicsPipeline> CreateGraphicsPipeline(
    const LogicalDevice &logicalDevice,
    const RenderPass &renderPass)
{
    auto vertexShaderData = Utils::ReadFile("./shaders/Shader.vert.spv");
    auto fragmentShaderData = Utils::ReadFile("./shaders/Shader.frag.spv");

    std::optional<ShaderModule> vertexShaderModule = CreateShaderModule(logicalDevice, vertexShaderData);
    std::optional<ShaderModule> fragmentShaderModule = CreateShaderModule(logicalDevice, fragmentShaderData);

    VkPipelineShaderStageCreateInfo vkVertexShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertexShaderModule->handle,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo vkFragmentShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragmentShaderModule->handle,
        .pName = "main"
    };

    std::vector<VkPipelineShaderStageCreateInfo> vkShaderStageCreateInfos = {
        vkVertexShaderStageCreateInfo,
        vkFragmentShaderStageCreateInfo
    };

    std::vector<VkDynamicState> vkDynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo vkDynamicStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(vkDynamicStates.size()),
        .pDynamicStates = vkDynamicStates.data()
    };

    VkPipelineVertexInputStateCreateInfo vkVertexInputCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = nullptr
    };

    VkPipelineInputAssemblyStateCreateInfo vkInputAssemblyCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    VkPipelineViewportStateCreateInfo vkViewportStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1
    };

    VkPipelineRasterizationStateCreateInfo vkRasterizerStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE
    };

    VkPipelineMultisampleStateCreateInfo vkMultisampleStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE
    };

    VkPipelineColorBlendAttachmentState vkColorBlendAttachmentState {
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
            | VK_COLOR_COMPONENT_G_BIT
            | VK_COLOR_COMPONENT_B_BIT
            | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo vkColorBlendStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &vkColorBlendAttachmentState
    };

    VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
    };

    VkPipelineLayout layout = VK_NULL_HANDLE;

    VkResult vkResult = vkCreatePipelineLayout(logicalDevice.handle, &vkPipelineLayoutCreateInfo, nullptr, &layout);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Pipeline Layout. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Graphics Pipeline Layout Created Successfully.\n";

    VkGraphicsPipelineCreateInfo vkPipelineCreateInfo {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(vkShaderStageCreateInfos.size()),
        .pStages = vkShaderStageCreateInfos.data(),
        .pVertexInputState = &vkVertexInputCreateInfo,
        .pInputAssemblyState = &vkInputAssemblyCreateInfo,
        .pViewportState = &vkViewportStateCreateInfo,
        .pRasterizationState = &vkRasterizerStateCreateInfo,
        .pMultisampleState = &vkMultisampleStateCreateInfo,
        .pColorBlendState = &vkColorBlendStateCreateInfo,
        .pDynamicState = &vkDynamicStateCreateInfo,
        .layout = layout,
        .renderPass = renderPass.handle,
        .subpass = 0
    };

    VkPipeline handle = VK_NULL_HANDLE;

    vkResult = vkCreateGraphicsPipelines(logicalDevice.handle, VK_NULL_HANDLE, 1, &vkPipelineCreateInfo, nullptr, &handle);
    if (vkResult != VK_SUCCESS) {
        std::cerr << "Failed to Create Vulkan Graphics Pipeline. Error Code: " << vkResult << "\n";
        return std::nullopt;
    }

    std::cout << "Vulkan Graphics Pipeline Created Successfully.\n";

    return GraphicsPipeline {
        .handle = handle,
        .layout = layout,
        .vertexShaderModule = vertexShaderModule,
        .fragmentShaderModule = fragmentShaderModule
    };
}

void DestroyGraphicsPipeline(VkDevice vkDevice, GraphicsPipeline &pipeline)
{
    DestroyShaderModule(vkDevice, pipeline.vertexShaderModule.value());
    DestroyShaderModule(vkDevice, pipeline.fragmentShaderModule.value());

    if (pipeline.handle != VK_NULL_HANDLE) {
        vkDestroyPipeline(vkDevice, pipeline.handle, nullptr);
        pipeline.handle = VK_NULL_HANDLE;
    }

    if (pipeline.layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(vkDevice, pipeline.layout, nullptr);
        pipeline.layout = VK_NULL_HANDLE;
    }
}
}
