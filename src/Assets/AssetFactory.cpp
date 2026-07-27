#include "AssetFactory.hpp"

#include "AssetRegistry.hpp"

namespace Engine::Assets {

std::unique_ptr<AssetRegistry> CreateAssetRegistry(Graphics::AllocatorContext &allocatorContext)
{
    return std::make_unique<AssetRegistry>(allocatorContext);
}

}
