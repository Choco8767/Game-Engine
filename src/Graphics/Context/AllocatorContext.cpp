#include "AllocatorContext.hpp"

#include "Graphics/Allocators/BufferAllocator.hpp"

namespace Engine::Graphics {

AllocatorContext::AllocatorContext(
    Passkey<AllocatorContext>,
    std::unique_ptr<BufferAllocator> bufferAllocator)
    : m_bufferAllocator(std::move(bufferAllocator))
{
}

AllocatorContext::~AllocatorContext()
{
    Destroy();
}

std::unique_ptr<AllocatorContext> AllocatorContext::Create(const CoreContext &coreContext)
{
    auto bufferAllocator = BufferAllocator::Create(coreContext);

    return std::make_unique<AllocatorContext>(
        Passkey<AllocatorContext> {},
        std::move(bufferAllocator));
}

void AllocatorContext::Destroy()
{
    m_bufferAllocator->Destroy();
}

// Getters
BufferAllocator &AllocatorContext::GetBufferAllocator() { return *m_bufferAllocator; }
const BufferAllocator &AllocatorContext::GetBufferAllocator() const { return *m_bufferAllocator; }

}
