#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "WindowAPI.hpp"
#include "WindowConstants.hpp"

typedef VkSurfaceKHR_T *VkSurfaceKHR;

namespace Engine::Window {

struct WindowFramebufferSize {
    int width = 0;
    int height = 0;
};

class Window {
public:
    virtual ~Window() = default;

    virtual bool Init(
        int width = DEFAULT_WIDTH,
        int height = DEFAULT_HEIGHT,
        const char *title = DEFAULT_TITLE) = 0;
    virtual void Destroy() = 0;

    virtual VkSurfaceKHR CreateVulkanWindowSurface(VkInstance vkInstance) = 0;

    virtual void Update() = 0;

    virtual bool ShouldClose() const = 0;
    virtual bool HasResized() const = 0;

    virtual int GetFramebufferWidth() const = 0;
    virtual int GetFramebufferHeight() const = 0;
    virtual WindowFramebufferSize GetFramebufferSize() const = 0;

    virtual std::vector<const char *> GetRequiredInstanceExtensions() const = 0;
};

std::unique_ptr<Window> CreateWindow(API api);

}
