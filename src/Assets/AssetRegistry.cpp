#include "AssetRegistry.hpp"

#include <format>
#include <stdexcept>

#include "Graphics/Context/AllocatorContext.hpp"

#include "Graphics/Allocators/BufferAllocator.hpp"

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
    for (std::size_t i = 0; i < m_meshes.size(); i++) {
        if (!m_meshLive[i])
            continue;

        m_allocatorContext.GetBufferAllocator().DestroyBuffer(m_meshes[i].vertexBuffer);
        m_allocatorContext.GetBufferAllocator().DestroyBuffer(m_meshes[i].indexBuffer);
    }

    m_meshes.clear();
    m_meshLive.clear();
    m_freeMeshes.clear();
}

MeshHandle AssetRegistry::CreateMesh(
    const std::vector<Graphics::Vertex> &vertices,
    const std::vector<std::uint32_t> &indices)
{
    if (vertices.empty() || indices.empty())
        throw std::runtime_error("Attempted to Create a Mesh with no Geometry.");

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

    MeshAsset mesh {
        .vertexBuffer = vertexBuffer,
        .indexBuffer = indexBuffer,
        .vertexCount = vertices.size(),
        .indexCount = indices.size()
    };

    MeshHandle handle {};

    if (!m_freeMeshes.empty()) {
        handle.id = m_freeMeshes.back();
        m_freeMeshes.pop_back();
        m_meshes[handle.id] = mesh;
    } else {
        handle.id = static_cast<std::uint32_t>(m_meshes.size());
        m_meshes.push_back(mesh);
    }

    m_meshLive.resize(m_meshes.size(), false);
    m_meshLive[handle.id] = true;

    return handle;
}

void AssetRegistry::DestroyMesh(MeshHandle handle)
{
    const MeshAsset &mesh = GetMesh(handle);

    m_allocatorContext.GetBufferAllocator().DestroyBuffer(mesh.vertexBuffer);
    m_allocatorContext.GetBufferAllocator().DestroyBuffer(mesh.indexBuffer);

    m_meshes[handle.id] = MeshAsset {};
    m_meshLive[handle.id] = false;
    m_freeMeshes.push_back(handle.id);
}

const MeshAsset &AssetRegistry::GetMesh(MeshHandle handle) const
{
    if (handle.id >= m_meshes.size())
        throw std::runtime_error(std::format("Invalid Mesh Handle ID: {}", handle.id));

    if (!m_meshLive[handle.id])
        throw std::runtime_error(std::format("Stale Mesh Handle ID: {}", handle.id));

    return m_meshes[handle.id];
}

}
