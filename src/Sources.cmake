set(SOURCES
    # App
    "App/main.cpp"
    "App/App.cpp"

    # Utils
    "Utils/ReadFile.cpp"

    # Window
    "Window/Window.cpp"
    "Window/GLFW/GLFWWindow.cpp"

    # Renderer
    "Graphics/Renderer.cpp"
    "Graphics/Vulkan/VulkanRenderer.cpp"
    "Graphics/Vulkan/Procedural/Core/VulkanInstance.cpp"
    "Graphics/Vulkan/Procedural/Core/VulkanDebugMessenger.cpp"
    "Graphics/Vulkan/Procedural/Core/VulkanSurface.cpp"
    "Graphics/Vulkan/Procedural/Core/VulkanPhysicalDevice.cpp"
    "Graphics/Vulkan/Procedural/Core/VulkanLogicalDevice.cpp"
    "Graphics/Vulkan/Procedural/Swapchain/VulkanSwapchain.cpp"
    "Graphics/Vulkan/Procedural/Pipeline/VulkanGraphicsPipeline.cpp"
    "Graphics/Vulkan/Procedural/Pipeline/VulkanShaderModule.cpp"
    "Graphics/Vulkan/Procedural/Rendering/VulkanRenderPass.cpp"
    "Graphics/Vulkan/Procedural/Rendering/VulkanFramebuffer.cpp"
    "Graphics/Vulkan/Procedural/Commands/VulkanCommandPool.cpp"
    "Graphics/Vulkan/Procedural/Commands/VulkanCommandBuffer.cpp"
    "Graphics/Vulkan/Procedural/Sync/VulkanSemaphore.cpp"
    "Graphics/Vulkan/Procedural/Sync/VulkanFence.cpp"
)
