# CMakeList.txt : projet CMake pour CookieEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
include_directories("${INC_DIR}/Resources/")
include_directories("${INC_DIR}/Gameplay/")
include_directories("${DEPS_INC_DIR}/ImGui/")
include_directories("${DEPS_INC_DIR}/ImGuizmo/")
include_directories("${DEPS_INC_DIR}/")
include_directories("${DEPS_INC_DIR}/assimp/")
include_directories("${DEPS_INC_DIR}/assimp/Compiler/")
include_directories("${DEPS_INC_DIR}/DirectXTex/")
include_directories("${DEPS_INC_DIR}/reactphysics3d/")
include_directories("${DEPS_INC_DIR}/Lua/")
include_directories("${DEPS_INC_DIR}/Sol/")
include_directories("${DEPS_INC_DIR}/FMOD/")

# Record SUB_SYS configuration (WIN32 for release en Windows to remove console)
IF(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Generate CMake cache for Release")
    IF(WIN32)
        set(SUB_SYS WIN32)
    ENDIF(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
ELSE()
    message(STATUS "Generate CMake cache for Debug")
    add_definitions(-DDEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)

# List source files
set (SRC_FILES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Editor.cpp"
    
    "${DEPS_SRC_DIR}/DirectXTex/DDSTextureLoader11.cpp"
    "${DEPS_SRC_DIR}/DirectXTex/WICTextureLoader11.cpp"
    
    "${DEPS_SRC_DIR}/ImGui/imgui.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_demo.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_draw.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_impl_dx11.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_impl_glfw.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_tables.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_widgets.cpp"
    "${DEPS_SRC_DIR}/ImGui/imgui_stdlib.cpp"

    "${DEPS_SRC_DIR}/ImGuizmo/ImCurveEdit.cpp"
    "${DEPS_SRC_DIR}/ImGuizmo/ImGradient.cpp"
    "${DEPS_SRC_DIR}/ImGuizmo/ImGuizmo.cpp"
    "${DEPS_SRC_DIR}/ImGuizmo/ImSequencer.cpp"

    "${SRC_DIR}/Core/Window.cpp"
    
    "${SRC_DIR}/Render/Renderer.cpp"
    "${SRC_DIR}/Render/DebugRenderer.cpp"
    "${SRC_DIR}/Render/RendererRemote.cpp"
    "${SRC_DIR}/Render/FrameBuffer.cpp"
    "${SRC_DIR}/Render/SkyBox.cpp"
    "${SRC_DIR}/Render/Light.cpp"
    "${SRC_DIR}/Render/GeometryPass.cpp"
    "${SRC_DIR}/Render/FBODrawer.cpp"
    
    "${SRC_DIR}/Resources/Loader.cpp" 
    "${SRC_DIR}/Resources/Map.cpp" 
    "${SRC_DIR}/Resources/ResourcesManager.cpp"
    "${SRC_DIR}/Resources/Mesh.cpp"
    "${SRC_DIR}/Resources/Texture.cpp"
    "${SRC_DIR}/Resources/Scene.cpp"
    "${SRC_DIR}/Resources/Serialization.cpp" 
    "${SRC_DIR}/Resources/SoundManager.cpp" 

    "${SRC_DIR}/UI/UIeditor.cpp"
    "${SRC_DIR}/UI/CustomImWidget.cpp"
    "${SRC_DIR}/UI/Widgets/UIwidgetBases.cpp"
    "${SRC_DIR}/UI/Widgets/UIwidgets.cpp" 
    "${SRC_DIR}/UI/Widgets/ConsoleWidget.cpp"
    "${SRC_DIR}/UI/Widgets/FileExplorerWidget.cpp"
    "${SRC_DIR}/UI/Widgets/GameportWidget.cpp"
    "${SRC_DIR}/UI/Widgets/HierarchyWidget.cpp"
    "${SRC_DIR}/UI/Widgets/InspectorWidget.cpp"
    "${SRC_DIR}/UI/Widgets/TextureEditorWidget.cpp"
    "${SRC_DIR}/UI/Widgets/ToolbarWidget.cpp"
    "${SRC_DIR}/UI/Widgets/ViewportWidget.cpp"
    
    "${SRC_DIR}/Physics/PhysicsSimulator.cpp"
    
    "${SRC_DIR}/ECS/ComponentEditor.cpp"
    "${SRC_DIR}/ECS/ComponentHandler.cpp"
    "${SRC_DIR}/ECS/ComponentModel.cpp" 
    "${SRC_DIR}/ECS/ComponentPhysics.cpp"
    "${SRC_DIR}/ECS/Coordinator.cpp"
    "${SRC_DIR}/ECS/EntityHandler.cpp"
    

    "${SRC_DIR}/Editor.cpp" 
    "${SRC_DIR}/Game.cpp")

set(DLL "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/assimp-vc142-mt.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/lua54.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/fmod.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/fmodL.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/fmodstudio.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/dll/fmodstudioL.dll")

set(LIB_DIR "${CookieEngine_SOURCE_DIR}/CookieEngine/libs")

# Ajoutez une source à l'exécutable de ce projet.
add_executable (CookieEngine ${SUB_SYS} ${SRC_FILES})

set_target_properties(CookieEngine PROPERTIES CXX_STANDARD 17)

target_include_directories(CookieEngine INTERFACE CXX_STANDARD 17)

set_property(TARGET CookieEngine PROPERTY CXX_STANDARD 17)
set_property(TARGET CookieEngine PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_features(CookieEngine PUBLIC cxx_std_17)
target_compile_features(CookieEngine PRIVATE cxx_std_17)

target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/d3d11.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/d3dcompiler.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/glfw3.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/assimp-vc142-mt.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/zlibstatic.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/liblua54.a")

IF(CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/reactphysics3d.lib")
    target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/fmod_vc.lib")
    target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/fmodstudio_vc.lib")
ELSE()
    target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/debug_reactphysics3d.lib")
    target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/fmodL_vc.lib")
    target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/fmodstudioL_vc.lib")
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)


# TODO: Ajoutez des tests et installez des cibles si nécessaire.

add_custom_command(TARGET CookieEngine POST_BUILD COMMAND 
    ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_SOURCE_DIR}/${RESOURCE_DIR} 
        ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_DIR})

add_custom_command(TARGET CookieEngine POST_BUILD COMMAND 
    ${CMAKE_COMMAND} -E copy
        ${DLL} $<TARGET_FILE_DIR:CookieEngine>)
