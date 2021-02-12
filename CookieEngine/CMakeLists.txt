# CMakeList.txt : projet CMake pour CookieEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(RESOURCE_DIR "Assets/")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories("${INC_DIR}/")
include_directories("${INC_DIR}/Core/")
include_directories("${INC_DIR}/Core/Math/")
include_directories("${INC_DIR}/UI/")
include_directories("${INC_DIR}/Render/")
include_directories("${INC_DIR}/ECS/")

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
    ${SRC_DIR}/main.cpp
    )

set(LIB_DIR "${CookieEngine_SOURCE_DIR}/CookieEngine/libs")

# Ajoutez une source à l'exécutable de ce projet.
add_executable (CookieEngine  ${SUB_SYS} ${SRC_FILES})

target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/d3d11.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/d3dcompiler.lib")
target_link_libraries(CookieEngine PRIVATE "${LIB_DIR}/glfw3.lib")

# TODO: Ajoutez des tests et installez des cibles si nécessaire.

add_custom_command(TARGET CookieEngine POST_BUILD COMMAND 
    ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_SOURCE_DIR}/${RESOURCE_DIR} 
        ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_DIR})