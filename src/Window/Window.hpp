#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "WindowAPI.hpp"
#include "WindowConstants.hpp"

typedef VkSurfaceKHR_T *VkSurfaceKHR;

namespace Engine {

class Window {
public:
    virtual ~Window() = default;

    virtual bool Init(
        int width = DEFAULT_WINDOW_WIDTH,
        int height = DEFAULT_WINDOW_HEIGHT,
        const char *title = DEFAULT_WINDOW_TITLE) = 0;
    virtual void Destroy() = 0;

    virtual std::optional<VkSurfaceKHR> CreateVulkanWindowSurface(VkInstance vkInstance) = 0;

    virtual void PollEvents() = 0;

    virtual bool ShouldClose() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    virtual std::vector<const char *> GetRequiredInstanceExtensions() const = 0;
};

std::unique_ptr<Window> CreateWindow(WindowAPI api);
}
