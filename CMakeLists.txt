cmake_minimum_required(VERSION 3.15)

project(oop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Treat warnings as errors by default
if(DEFINED WARNINGS_AS_ERRORS)
    # already defined
elseif(DEFINED ENV{WARNINGS_AS_ERRORS})
    set(WARNINGS_AS_ERRORS $ENV{WARNINGS_AS_ERRORS})
else()
    set(WARNINGS_AS_ERRORS ON)
endif()

# Main executable
add_executable(${PROJECT_NAME} main.cpp src/Component.cpp src/Directory.cpp src/File.cpp src/ImageFile.cpp
src/VideoFile.cpp  src/EncryptedFile.cpp src/CompressedFile.cpp src/SecureArchive.cpp
src/Manager.cpp src/VFSInterface.cpp
external/imgui/imgui-master/imgui.cpp
external/imgui/imgui-master/imgui_draw.cpp
external/imgui/imgui-master/imgui_tables.cpp
external/imgui/imgui-master/imgui_widgets.cpp
external/imgui/imgui-master/backends/imgui_impl_glfw.cpp
external/imgui/imgui-master/backends/imgui_impl_opengl3.cpp
external/imgui/imgui-master/misc/cpp/imgui_stdlib.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui-master
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui-master/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui-master/misc/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/glfw-3.4.bin.WIN64/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/glfw-3.4.bin.WIN64/lib-vc2022
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw3dll opengl32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

if(GITHUB_ACTIONS)
    message("NOTE: GITHUB_ACTIONS defined")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GITHUB_ACTIONS)
endif()

# Compiler flags
message("Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /wd4244 /wd4267 /wd4996 /external:anglebrackets /external:W0)
    set_source_files_properties(
        external/imgui/imgui-master/imgui.cpp
        external/imgui/imgui-master/imgui_draw.cpp
        external/imgui/imgui-master/imgui_tables.cpp
        external/imgui/imgui-master/imgui_widgets.cpp
        external/imgui/imgui-master/backends/imgui_impl_glfw.cpp
        external/imgui/imgui-master/backends/imgui_impl_opengl3.cpp
        external/imgui/imgui-master/misc/cpp/imgui_stdlib.cpp
        PROPERTIES COMPILE_OPTIONS "/W0"
    )
    if(WARNINGS_AS_ERRORS)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
    set_source_files_properties(
        external/imgui/imgui-master/imgui.cpp
        external/imgui/imgui-master/imgui_draw.cpp
        external/imgui/imgui-master/imgui_tables.cpp
        external/imgui/imgui-master/imgui_widgets.cpp
        external/imgui/imgui-master/backends/imgui_impl_glfw.cpp
        external/imgui/imgui-master/backends/imgui_impl_opengl3.cpp
        external/imgui/imgui-master/misc/cpp/imgui_stdlib.cpp
        PROPERTIES COMPILE_OPTIONS "-w"
    )
    if(WARNINGS_AS_ERRORS)
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

# Sanitizers Configuration
set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:/fsanitize=address>")
elseif(APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
elseif(UNIX)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        # We leave GCC "clean" so Valgrind can run without shadow memory conflicts
        message("GCC detected: Sanitizers disabled to allow Valgrind usage.")
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        # Enable ASan for Clang 14 and all newer versions (15, 16, 17, 18, etc.)
        if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "14.0")
            target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            target_link_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
        else()
            message("Legacy Clang detected: Sanitizers skipped.")
        endif()
    endif()
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)