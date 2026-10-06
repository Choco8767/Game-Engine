#pragma once

#include <memory>

#include "Utils/Passkey.hpp"

#include "Graphics/Allocators/BufferAllocator.hpp"
#include "Graphics/Context/AllocatorContext.hpp"
#include "Graphics/Vulkan/Allocators/VulkanBufferAllocator.hpp"

namespace Engine::Graphics::Vulkan {

class CoreContextBackend;

class AllocatorContextBackend final : public AllocatorContext {
public:
    AllocatorContextBackend(
        Passkey<AllocatorContextBackend>,
        std::unique_ptr<BufferAllocatorBackend> bufferAllocator);
    ~AllocatorContextBackend() override;

    static std::unique_ptr<AllocatorContextBackend> Create(const CoreContextBackend &coreContext);
    void Destroy() override;

    // Getters
    API GetAPIType() const noexcept override { return API::VULKAN; }

    BufferAllocatorBackend &GetBufferAllocator() override;
    const BufferAllocatorBackend &GetBufferAllocator() const override;

private:
    std::unique_ptr<BufferAllocatorBackend> m_bufferAllocator;
};

}
