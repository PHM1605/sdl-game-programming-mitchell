cmake_minimum_required(VERSION 3.0.0)
project(running_diag)
set(SourceFiles
    main.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

add_executable(${PROJECT_NAME}
    ${SourceFiles}
)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR} # so Game.h is found; or #include "lib/Game.h" if in sub-dir
)

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS} 
    ${SDL2_TTF_LIBRARY_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${SDL_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)