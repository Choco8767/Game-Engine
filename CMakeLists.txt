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

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "."
)
