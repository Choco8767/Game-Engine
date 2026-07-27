set(SOURCES
    # App
    "App/main.cpp"
    "App/App.cpp"

    # Utils
    "Utils/ReadFile.cpp"

    # Window
    "Window/Window.cpp"
    "Window/GLFW/GLFWWindow.cpp"

    # Assets
    "Assets/AssetFactory.cpp"
    "Assets/AssetRegistry.cpp"

    # Renderer
    "Graphics/Context/Context.cpp"
    "Graphics/Context/CoreContext.cpp"
    "Graphics/Context/RenderContext.cpp"
    "Graphics/Context/AllocatorContext.cpp"
    "Graphics/Allocators/BufferAllocator.cpp"
    "Graphics/Vulkan/Context/VulkanCoreContext.cpp"
    "Graphics/Vulkan/Context/VulkanRenderContext.cpp"
    "Graphics/Vulkan/Allocators/VulkanBufferAllocator.cpp"
    "Graphics/Vulkan/VulkanRenderer.cpp"
    "Graphics/Vulkan/Helpers/VulkanBufferTypes.cpp"
    "Graphics/Vulkan/Helpers/VulkanVertexHelpers.cpp"
    "Graphics/Vulkan/Internal/Core/VulkanInstance.cpp"
    "Graphics/Vulkan/Internal/Core/VulkanDebugMessenger.cpp"
    "Graphics/Vulkan/Internal/Core/VulkanSurface.cpp"
    "Graphics/Vulkan/Internal/Core/VulkanPhysicalDevice.cpp"
    "Graphics/Vulkan/Internal/Core/VulkanLogicalDevice.cpp"
    "Graphics/Vulkan/Internal/Swapchain/VulkanSwapchain.cpp"
    "Graphics/Vulkan/Internal/Pipeline/VulkanGraphicsPipeline.cpp"
    "Graphics/Vulkan/Internal/Pipeline/VulkanShaderModule.cpp"
    "Graphics/Vulkan/Internal/Rendering/VulkanRenderPass.cpp"
    "Graphics/Vulkan/Internal/Rendering/VulkanFramebuffer.cpp"
    "Graphics/Vulkan/Internal/Commands/VulkanCommandPool.cpp"
    "Graphics/Vulkan/Internal/Commands/VulkanCommandBuffer.cpp"
    "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorPool.cpp"
    "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorSetLayoutRegistry.cpp"
    "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorSetLayoutBindings.cpp"
    "Graphics/Vulkan/Internal/Descriptors/VulkanDescriptorWriter.cpp"
    "Graphics/Vulkan/Internal/Sync/VulkanSemaphore.cpp"
    "Graphics/Vulkan/Internal/Sync/VulkanFence.cpp"
    "Graphics/Vulkan/Internal/Memory/VulkanMemoryAllocator.cpp"
    "Graphics/Vulkan/Internal/Memory/VulkanMemoryAllocation.cpp"
)
