#pragma once

#include <cstdint>
#include <vector>

#include "Graphics/Types/GraphicsHandles.hpp"

namespace Engine::Graphics::Vulkan {

struct LogicalDevice;

struct DescriptorSetLayout;
struct DescriptorSetLayoutBindings;

class DescriptorSetLayoutRegistry {
public:
    DescriptorSetLayoutRegistry(const LogicalDevice &logicalDevice);
    ~DescriptorSetLayoutRegistry();

    void Destroy();

    DescriptorSetLayoutHandle CreateDescriptorSetLayout(const DescriptorSetLayoutBindings &bindings);
    void DestroyDescriptorSetLayout(DescriptorSetLayoutHandle DescriptorSetLayout);

    // Getters
    const DescriptorSetLayout &GetDescriptorSetLayout(DescriptorSetLayoutHandle descriptorSetLayout) const;

private:
    const LogicalDevice &m_logicalDevice;

    std::vector<DescriptorSetLayout> m_descriptorSetLayouts;
    std::vector<std::size_t> m_freeDescriptorSetLayouts;
};

}
