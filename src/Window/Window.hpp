#pragma once

#include <memory>
#include <vector>

#include "WindowAPI.hpp"
#include "WindowConstants.hpp"

namespace Engine {

class Window {
public:
    virtual ~Window() = default;

    virtual bool Init(
        int width = DEFAULT_WINDOW_WIDTH,
        int height = DEFAULT_WINDOW_HEIGHT,
        const char *title = DEFAULT_WINDOW_TITLE) = 0;
    virtual void Destroy() = 0;

    virtual void PollEvents() = 0;

    virtual bool ShouldClose() const = 0;

    virtual std::vector<const char *> GetRequiredInstanceExtensions() const = 0;
};

std::unique_ptr<Window> CreateWindow(WindowAPI api);

}
