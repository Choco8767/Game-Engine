find_program(DXC_EXECUTABLE dxc HINTS "$ENV{VK_SDK_PATH}/Bin")
if(NOT DXC_EXECUTABLE)
    message(FATAL_ERROR "Could not find DXC shader compiler in Vulkan SDK.")
endif()

file(GLOB_RECURSE HLSL_SHADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hlsl")
set(SPIRV_SHADERS "")

foreach(SHADER ${HLSL_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} "NAME_WE")
    get_filename_component(SHADER_EXT ${SHADER} "EXT")

    if(SHADER_EXT MATCHES ".vs.hlsl")
        set(TARGET_PROFILE "vs_6_0")
    elseif(SHADER_EXT MATCHES ".ps.hlsl")
        set(TARGET_PROFILE "ps_6_0")
    else()
        message(FATAL_ERROR "Unknown Shader Stage for ${SHADER_NAME}. Ensure the Extension is '.vs.hlsl' or '.ps.hlsl'")
    endif()

    # Output Compiled Binaries to Install Directories
    set(OUT_FILE "${INSTALL_DIRECTORY}/$<CONFIG>/assets/shaders/${SHADER_NAME}.spv")
    set(OUT_DIR  "${INSTALL_DIRECTORY}/$<CONFIG>/assets/shaders")

    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUT_DIR}
        COMMAND ${DXC_EXECUTABLE} -T ${TARGET_PROFILE} -E main -Fo ${OUT_FILE} ${SHADER} -spirv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER} to SPIR-V (${TARGET_PROFILE})"
        VERBATIM
    )
    list(APPEND SPIRV_SHADERS "${OUT_FILE}")
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_SHADERS})
add_dependencies(${PROJECT_NAME} Shaders)
