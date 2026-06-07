#pragma once

#include <memory>

namespace Engine::Graphics {

class Allocator;

}

namespace Engine::Assets {

class AssetRegistry;

std::unique_ptr<AssetRegistry> CreateAssetRegistry(Graphics::Allocator &allocator);

}
