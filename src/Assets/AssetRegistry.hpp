#pragma once

#include <cstdint>
#include <vector>

#include "Assets/Types/AssetHandles.hpp"
#include "Assets/Types/GraphicsMesh.hpp"

namespace Engine::Graphics {

class Allocator;

struct Vertex;

}

namespace Engine::Assets {

struct GraphicsMesh;

class AssetRegistry {
public:
    AssetRegistry(Graphics::Allocator &allocator);
    ~AssetRegistry();

    void Destroy();

    MeshHandle CreateMesh(
        const std::vector<Graphics::Vertex> &vertices,
        const std::vector<uint32_t> &indices);
    void DestroyMesh(MeshHandle mesh);

    // Getters
    const GraphicsMesh &GetMesh(MeshHandle mesh) const { return m_meshes[mesh.id]; }

private:
    Graphics::Allocator &m_allocator;

    std::vector<GraphicsMesh> m_meshes;
    std::vector<std::size_t> m_freeMeshes;
};

}
