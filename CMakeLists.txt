# CMakeList.txt : CMake project for game-scene, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("game-scene")

set(SHARED_SOURCES 
"sdlwindow.cpp" "sdlwindow.h" "geometry.h" "geometry.cpp" "label.h" "label.cpp" "sdlapp.h" "sdlapp.cpp"  "sdlwrappers.h" "sdltexture.h" "sdltexture.cpp" "constants.h" "CLI11.hpp")

# demo
add_executable (game-scene 
    "game-scene.cpp" 
    "game-scene.h"
    ${SHARED_SOURCES}
)

# collision system
add_executable (collision-system
    "collision-system.cpp"
    "collision-system.h"
    "game-scene.h"
    "particle.h"
    "particle.cpp"
    ${SHARED_SOURCES})

find_package(SDL2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

target_link_libraries(game-scene
    PRIVATE
    fmt::fmt
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

target_link_libraries(collision-system
    PRIVATE
    fmt::fmt
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

set_property(TARGET game-scene PROPERTY CXX_STANDARD 20)
set_property(TARGET collision-system PROPERTY CXX_STANDARD 20)

file(COPY "textures" DESTINATION .)

# TODO: Add tests and install targets if needed.
