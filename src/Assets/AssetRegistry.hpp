#pragma once

#include <cstdint>
#include <vector>

#include "Assets/Types/AssetHandles.hpp"
#include "Assets/Types/MeshAsset.hpp"

namespace Engine::Graphics {

class AllocatorContext;

struct Vertex;

}

namespace Engine::Assets {

class AssetRegistry {
public:
    AssetRegistry(Graphics::AllocatorContext &allocatorContext);
    ~AssetRegistry();

    void Destroy();

    MeshHandle CreateMesh(
        const std::vector<Graphics::Vertex> &vertices,
        const std::vector<std::uint32_t> &indices);
    void DestroyMesh(MeshHandle mesh);

    // Getters
    const MeshAsset &GetMesh(MeshHandle mesh) const;

private:
    Graphics::AllocatorContext &m_allocatorContext;

    std::vector<MeshAsset> m_meshes;
    std::vector<bool> m_meshLive;
    std::vector<std::uint32_t> m_freeMeshes;
};

}
