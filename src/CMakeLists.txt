target_sources(${PROJECT_NAME} PRIVATE
    # App
    "App/main.cpp"
    "App/App.cpp"

    # Window
    "Window/Window.cpp"
    "Window/GLFW/GLFWWindow.cpp"

    "Graphics/Vulkan/VulkanRenderer.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm::glm
    Vulkan::Vulkan
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLM_ENABLE_EXPERIMENTAL
)
