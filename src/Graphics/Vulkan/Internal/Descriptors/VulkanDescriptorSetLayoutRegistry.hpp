#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "Graphics/Types/GraphicsHandles.hpp"

#include "VulkanDescriptorSetLayout.hpp"

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct DescriptorSetLayout;
struct DescriptorSetLayoutBindings;

class DescriptorSetLayoutRegistry {
public:
    DescriptorSetLayoutRegistry(const LogicalDevice &logicalDevice);
    ~DescriptorSetLayoutRegistry();

    DescriptorSetLayoutRegistry(const DescriptorSetLayoutRegistry &other) = delete;
    DescriptorSetLayoutRegistry &operator=(const DescriptorSetLayoutRegistry &other) = delete;

    DescriptorSetLayoutRegistry(DescriptorSetLayoutRegistry &&other) noexcept = default;
    DescriptorSetLayoutRegistry &operator=(DescriptorSetLayoutRegistry &&other) noexcept = default;

    void Destroy();

    DescriptorSetLayoutHandle CreateDescriptorSetLayout(const DescriptorSetLayoutBindings &bindings);
    void DestroyDescriptorSetLayout(DescriptorSetLayoutHandle DescriptorSetLayout);

    // Getters
    const DescriptorSetLayout &GetDescriptorSetLayout(DescriptorSetLayoutHandle descriptorSetLayout) const;

private:
    std::reference_wrapper<const LogicalDevice> m_logicalDevice;

    std::vector<DescriptorSetLayout> m_descriptorSetLayouts;
    std::vector<std::size_t> m_freeDescriptorSetLayouts;
};

}
