#include "VulkanAllocatorContext.hpp"

#include "Graphics/Vulkan/Allocators/VulkanBufferAllocator.hpp"

namespace Engine::Graphics::Vulkan {

AllocatorContextBackend::AllocatorContextBackend(
    Passkey<AllocatorContextBackend>,
    std::unique_ptr<BufferAllocatorBackend> bufferAllocator)
    : m_bufferAllocator(std::move(bufferAllocator))
{
}

AllocatorContextBackend::~AllocatorContextBackend()
{
    Destroy();
}

std::unique_ptr<AllocatorContextBackend> AllocatorContextBackend::Create(const CoreContextBackend &coreContext)
{
    auto bufferAllocator = BufferAllocatorBackend::Create(coreContext);

    return std::make_unique<AllocatorContextBackend>(
        Passkey<AllocatorContextBackend> {},
        std::move(bufferAllocator));
}

void AllocatorContextBackend::Destroy()
{
    m_bufferAllocator->Destroy();
}

// Getters
BufferAllocatorBackend &AllocatorContextBackend::GetBufferAllocator() { return *m_bufferAllocator; }
const BufferAllocatorBackend &AllocatorContextBackend::GetBufferAllocator() const { return *m_bufferAllocator; }

}
