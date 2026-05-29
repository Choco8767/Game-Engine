include("${CMAKE_CURRENT_SOURCE_DIR}/Sources.cmake")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

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
