#include "AssetFactory.hpp"

#include "AssetRegistry.hpp"

namespace Engine::Assets {

std::unique_ptr<AssetRegistry> CreateAssetRegistry(Graphics::Allocator &allocator)
{
    return std::make_unique<AssetRegistry>(allocator);
}

}
