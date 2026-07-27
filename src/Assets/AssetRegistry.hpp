#pragma once

#include <cstdint>
#include <vector>

#include "Assets/Types/AssetHandles.hpp"
#include "Assets/Types/GraphicsMesh.hpp"

namespace Engine::Graphics {

class AllocatorContext;

struct Vertex;

}

namespace Engine::Assets {

struct GraphicsMesh;

class AssetRegistry {
public:
    AssetRegistry(Graphics::AllocatorContext &allocatorContext);
    ~AssetRegistry();

    void Destroy();

    MeshHandle CreateMesh(
        const std::vector<Graphics::Vertex> &vertices,
        const std::vector<uint32_t> &indices);
    void DestroyMesh(MeshHandle mesh);

    // Getters
    const GraphicsMesh &GetMesh(MeshHandle mesh) const { return m_meshes[mesh.id]; }

private:
    Graphics::AllocatorContext &m_allocatorContext;

    std::vector<GraphicsMesh> m_meshes;
    std::vector<std::size_t> m_freeMeshes;
};

}
