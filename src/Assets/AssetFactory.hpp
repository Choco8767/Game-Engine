#pragma once

#include <memory>

namespace Engine::Graphics {

class AllocatorContext;

}

namespace Engine::Assets {

class AssetRegistry;

std::unique_ptr<AssetRegistry> CreateAssetRegistry(Graphics::AllocatorContext &allocatorContext);

}
