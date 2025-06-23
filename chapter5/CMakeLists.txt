cmake_minimum_required(VERSION 3.0.0)
project(sdl-game-programming-mitchell)
set(SourceFiles 
    main.cpp
    Game.cpp
    TextureManager.cpp
    Player.cpp 
    Enemy.cpp
    SDLGameObject.cpp
	Vector2D.cpp
    InputHandler.cpp
    MenuState.cpp 
    PlayState.cpp 
    GameStateMachine.cpp
    MenuButton.cpp
    PauseState.cpp
    AnimatedGraphic.cpp
    GameOverState.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

add_executable(${PROJECT_NAME} 
    ${SourceFiles}
)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR} # So Game.h is found
)

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES}
)
