# Find Compilers

find_program(DXC_EXECUTABLE dxc HINTS "$ENV{VULKAN_SDK}/bin")
find_program(GLSLC_EXECUTABLE glslc HINTS "$ENV{VULKAN_SDK}/bin")

if(NOT DXC_EXECUTABLE OR NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "Could not find shader compilers in Vulkan SDK.")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/Shaders.cmake")

set(SPIRV_SHADERS "")

foreach(SHADER_FILE ${SHADERS})
    set(SHADER_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}")

    get_filename_component(SHADER_FILENAME ${SHADER_SOURCE_PATH} NAME)
    get_filename_component(SHADER_EXT ${SHADER_FILE} EXT)

    set(OUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_FILENAME}.spv")

    if(SHADER_EXT STREQUAL ".hlsl")
        if(SHADER_FILE MATCHES ".*VS\\.hlsl$")
            set(TARGET_PROFILE "vs_6_0")
        elseif(SHADER_FILE MATCHES ".*PS\\.hlsl$")
            set(TARGET_PROFILE "ps_6_0")
        else()
            message(FATAL_ERROR "Unknown HLSL Stage for ${SHADER_FILE}")
        endif()

        add_custom_command(
            OUTPUT "${OUT_FILE}"
            COMMAND ${DXC_EXECUTABLE} -T ${TARGET_PROFILE} -E main -Fo "${OUT_FILE}" "${SHADER_SOURCE_PATH}" -spirv
            DEPENDS "${SHADER_SOURCE_PATH}"
            COMMENT "Compiling HLSL: ${SHADER_FILE} -> ${SHADER_FILENAME}.spv"
            VERBATIM
        )

    elseif(SHADER_EXT STREQUAL ".vert" OR SHADER_EXT STREQUAL ".frag")
        add_custom_command(
            OUTPUT "${OUT_FILE}"
            COMMAND ${GLSLC_EXECUTABLE} -o "${OUT_FILE}" "${SHADER_SOURCE_PATH}"
            DEPENDS "${SHADER_SOURCE_PATH}"
            COMMENT "Compiling GLSL: ${SHADER_FILE} -> ${SHADER_FILENAME}.spv"
            VERBATIM
        )
    else()
        message(FATAL_ERROR "Unsupported shader extension: ${SHADER_EXT}")
    endif()

    list(APPEND SPIRV_SHADERS "${OUT_FILE}")
endforeach()

# Custom Target
add_custom_target(CompileShaders ALL DEPENDS ${SPIRV_SHADERS})

if(TARGET ${PROJECT_NAME})
    add_dependencies(${PROJECT_NAME} CompileShaders)
endif()

# Installation
install(FILES ${SPIRV_SHADERS}
        DESTINATION "shaders"
        COMPONENT Shaders)
