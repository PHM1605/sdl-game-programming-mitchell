cmake_minimum_required(VERSION 3.0.0)
project(sdl-game-programming-mitchell)
set(SourceFiles 
    base64.cpp
    Camera.cpp
    CollisionManager.cpp
    Game.cpp 
    GameObjectFactory.cpp
    GameStateMachine.cpp
    InputHandler.cpp
    Level.cpp
    LevelParser.cpp
    main.cpp
    MainMenuState.cpp
    MenuButton.cpp
    ObjectLayer.cpp
    PlatformerObject.cpp 
    Player.cpp
    PlayState.cpp
    ScrollingBackground.cpp
    SoundManager.cpp
    StateParser.cpp 
    TextureManager.cpp
    TileLayer.cpp
    tinystr.cpp 
    tinyxml.cpp 
    tinyxmlerror.cpp 
    tinyxmlparser.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME} 
    ${SourceFiles}
)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR} # So Game.h is found
    ${ZLIB_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${ZLIB_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)
