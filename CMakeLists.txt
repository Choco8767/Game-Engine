cmake_minimum_required(VERSION 3.26)

project(Game-Engine LANGUAGES C CXX)

# Enable Hot Reload for MSVC Compilers
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
endif()

# C++ Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(glfw3  CONFIG REQUIRED)
find_package(glm    CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Executable
add_executable(${PROJECT_NAME})

# Subdirectories
add_subdirectory(src)
add_subdirectory(shaders)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm::glm
    Vulkan::Vulkan
)

# Add Defines
target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLM_ENABLE_EXPERIMENTAL
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "."
)
install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    DESTINATION ".")
