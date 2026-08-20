
#pragma once

#include <cstdint>

namespace Engine::Graphics {

enum class ShaderStage : std::uint8_t {
    NONE = 0,
    VERTEX = 1 << 0,
    FRAGMENT = 1 << 1,
    GEOMETRY = 1 << 2,
    COMPUTE = 1 << 3,

    ALL = VERTEX | FRAGMENT | GEOMETRY | COMPUTE
};

inline constexpr ShaderStage operator|(ShaderStage lhs, ShaderStage rhs)
{
    return static_cast<ShaderStage>(
        static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

inline constexpr ShaderStage &operator|=(ShaderStage &lhs, ShaderStage rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

inline constexpr ShaderStage operator&(ShaderStage lhs, ShaderStage rhs)
{
    return static_cast<ShaderStage>(
        static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
}

inline constexpr ShaderStage &operator&=(ShaderStage &lhs, ShaderStage rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

}
