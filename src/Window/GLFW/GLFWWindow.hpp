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
    void Destroy() override;

    void PollEvents() override;

    bool ShouldClose() const override;

    std::vector<const char *> GetRequiredInstanceExtensions() const override;

private:
    GLFWwindow *m_handle;
};

}
