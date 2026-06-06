#pragma once

#include <cstdint>

template <typename T>
struct Handle {
    uint32_t id = 0;

    bool operator==(const Handle &other) const { return id == other.id; }
    bool operator!=(const Handle &other) const { return id != other.id; }
};
