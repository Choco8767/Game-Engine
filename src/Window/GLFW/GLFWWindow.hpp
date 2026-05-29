#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Window.hpp"

namespace Engine::GLFW {

class WindowBackend final : public Engine::Window {
public:
    WindowBackend() = default;
    ~WindowBackend() override;

    bool Init(
        int width,
        int height,
        const char *title) override;

    std::optional<VkSurfaceKHR> CreateVulkanWindowSurface(VkInstance vkInstance) override;

    void Destroy() override;

    void PollEvents() override;

    bool ShouldClose() const override;
    int GetWidth() const override;
    int GetHeight() const override;

    std::vector<const char *> GetRequiredInstanceExtensions() const override;

private:
    GLFWwindow *m_handle;
};

}
