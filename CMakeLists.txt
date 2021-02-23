# CMakeList.txt : fichier projet CMake de niveau supérieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required (VERSION 3.8)

project ("CookieEngine")

set(SOURCE_DIR "CookieEngine")


include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Incluez les sous-projets.
add_subdirectory (${SOURCE_DIR})