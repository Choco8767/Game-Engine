#include "AssetRegistry.hpp"

#include "Graphics/Allocators/BufferAllocator.hpp"
#include "Graphics/Context/AllocatorContext.hpp"
#include "Graphics/Core/Vertex.hpp"
#include "Graphics/Types/BufferTypes.hpp"

namespace Engine::Assets {

AssetRegistry::AssetRegistry(Graphics::AllocatorContext &allocatorContext)
    : m_allocatorContext(allocatorContext)
{
}

AssetRegistry::~AssetRegistry()
{
    Destroy();
}

void AssetRegistry::Destroy()
{

    for (const auto &mesh : m_meshes) {
        m_allocatorContext.GetBufferAllocator().DestroyBuffer(mesh.vertexBuffer);
        m_allocatorContext.GetBufferAllocator().DestroyBuffer(mesh.indexBuffer);
    }

    m_meshes.clear();
    m_freeMeshes.clear();
}

MeshHandle AssetRegistry::CreateMesh(
    const std::vector<Graphics::Vertex> &vertices,
    const std::vector<std::uint32_t> &indices)
{
    Engine::Graphics::BufferCreateInfo vertexBufferCreateInfo {
        .size = vertices.size() * sizeof(Engine::Graphics::Vertex),
        .usage = Engine::Graphics::BufferUsage::VERTEX
    };
    Engine::Graphics::BufferCreateInfo indexBufferCreateInfo {
        .size = indices.size() * sizeof(std::uint32_t),
        .usage = Engine::Graphics::BufferUsage::INDEX
    };

    BufferHandle vertexBuffer = m_allocatorContext.GetBufferAllocator().CreateBuffer(vertexBufferCreateInfo, vertices.data());
    BufferHandle indexBuffer = m_allocatorContext.GetBufferAllocator().CreateBuffer(indexBufferCreateInfo, indices.data());

    GraphicsMesh mesh {
        .vertexBuffer = vertexBuffer,
        .indexBuffer = indexBuffer,
        .vertexCount = vertices.size(),
        .indexCount = indices.size()
    };

    MeshHandle handle {};

    if (!m_freeMeshes.empty()) {
        handle.id = static_cast<std::uint32_t>(m_freeMeshes.back());
        m_freeMeshes.pop_back();
        m_meshes[handle.id] = mesh;
    } else {
        handle.id = static_cast<std::uint32_t>(m_meshes.size());
        m_meshes.push_back(mesh);
    }

    return handle;
}

void AssetRegistry::DestroyMesh(MeshHandle mesh)
{
    const auto &rawMesh = GetMesh(mesh);

    m_allocatorContext.GetBufferAllocator().DestroyBuffer(rawMesh.vertexBuffer);
    m_allocatorContext.GetBufferAllocator().DestroyBuffer(rawMesh.indexBuffer);
}

}
