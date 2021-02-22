# CMakeList.txt : projet CMake pour CookieEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(RESOURCE_DIR "Assets/")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DEPS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/src")
set(DEPS_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/include")

include_directories("${INC_DIR}/")
include_directories("${INC_DIR}/Core/")
include_directories("${INC_DIR}/Core/Math/")
include_directories("${INC_DIR}/UI/")
include_directories("${INC_DIR}/UI/Widgets/")
include_directories("${INC_DIR}/Render/")
include_directories("${INC_DIR}/ECS/")
include_directories("${DEPS_INC_DIR}/ImGui/")
include_directories("${DEPS_INC_DIR}/")
include_directories("${DEPS_INC_DIR}/assimp/")
include_directories("${DEPS_INC_DIR}/assimp/Compiler/")

# Record SUB_SYS configuration (WIN32 for release en Windows to remove console)
IF(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Generate CMake cache for Release")
    IF(WIN32)
        set(SUB_SYS WIN32)
    ENDIF(WIN32)
ELSE()
    message(STATUS "Generate CMake cache for Debug")
    add_definitions(-DDEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)

# List source files
set (SRC_FILES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Engine.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_demo.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_draw.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_impl_dx11.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_impl_glfw.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_tables.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_widgets.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_stdlib.cpp"
    "${SRC_DIR}/Core/Window.cpp"
    "${SRC_DIR}/Render/Renderer.cpp"
    "${SRC_DIR}/Render/RendererRemote.cpp"
    "${SRC_DIR}/Render/RendererState.cpp" 
    "${SRC_DIR}/Resources/Loader.cpp" 
    "${SRC_DIR}/Resources/ResourcesManager.cpp"
    "${SRC_DIR}/Resources/Mesh.cpp"
    "${SRC_DIR}/UI/UIcore.cpp"
    "${SRC_DIR}/UI/Widgets/UIwidgetBases.cpp"
    "${SRC_DIR}/UI/Widgets/UIwidgets.cpp" 
    "${SRC_DIR}/Resources/Shader.cpp"
    "${SRC_DIR}/UI/Widgets/FileExplorerWidget.cpp"
    "${SRC_DIR}/UI/Widgets/HierarchyWidget.cpp"
    "${SRC_DIR}/UI/Widgets/InspectorWidget.cpp"
    )

set(LIB_DIR "${CookieEngine_SOURCE_DIR}/CookieEngine/libs")

# Ajoutez une source à l'exécutable de ce projet.
add_executable (CookieEngine  ${SUB_SYS} ${SRC_FILES})

target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/d3d11.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/d3dcompiler.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/glfw3.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/assimp-vc142-mt.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/zlibstatic.lib")

# TODO: Ajoutez des tests et installez des cibles si nécessaire.

add_custom_command(TARGET CookieEngine POST_BUILD COMMAND 
    ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_SOURCE_DIR}/${RESOURCE_DIR} 
        ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_DIR})