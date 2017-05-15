cmake_minimum_required(VERSION 3.4.1)
project(Gish)

# Set CMake-modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Deploy Game Files and Executable.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../../../assets_full/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

option(GLES "Enable OpenGL ES instead OpenGL" OFF)

find_package(SDL2 REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vorbis REQUIRED)
find_package(PNG REQUIRED)

include_directories(${PROJECT_NAME}
                    ${SDL2_INCLUDE_DIR}
                    ${OPENAL_INCLUDE_DIR}
                    ${OPENGL_INCLUDE_DIR}
                    ${Vorbis_INCLUDE_DIR}
                    ${PNG_INCLUDE_DIR})

if(GLES)
    message(STATUS "Build w/ using OpenGLES libraries!")
    set(TARGET_NAME "Gish-gles")
    find_package(OpenGLES REQUIRED)
    add_definitions(-DGLES -DPC_GLES)
    include_directories(${INCLUDE_DIRECTORIES} ${OPENGLES_INCLUDE_DIR})
else()
    message(STATUS "Build w/ using OpenGL libraries!")
    set(TARGET_NAME ${PROJECT_NAME})
endif()

set(GISH_SOURCES    ${PROJECT_NAME}/audio/audio.c
                    ${PROJECT_NAME}/game/ai.c
                    ${PROJECT_NAME}/game/animation.c
                    ${PROJECT_NAME}/game/block.c
                    ${PROJECT_NAME}/game/boss.c
                    ${PROJECT_NAME}/game/config.c
                    ${PROJECT_NAME}/game/credits.c
                    ${PROJECT_NAME}/game/custom.c
                    ${PROJECT_NAME}/game/damage.c
                    ${PROJECT_NAME}/game/debug.c
                    ${PROJECT_NAME}/game/editor.c
                    ${PROJECT_NAME}/game/game.c
                    ${PROJECT_NAME}/game/gameaudio.c
                    ${PROJECT_NAME}/game/gamemenu.c
                    ${PROJECT_NAME}/game/gameobject.c
                    ${PROJECT_NAME}/game/gametexture.c
                    ${PROJECT_NAME}/game/high.c
                    ${PROJECT_NAME}/game/level.c
                    ${PROJECT_NAME}/game/lighting.c
                    ${PROJECT_NAME}/game/logic.c
                    ${PROJECT_NAME}/game/mainmenu.c
                    ${PROJECT_NAME}/game/mappack.c
                    ${PROJECT_NAME}/game/music.c
                    ${PROJECT_NAME}/game/objedit.c
                    ${PROJECT_NAME}/game/objfunc.c
                    ${PROJECT_NAME}/game/options.c
                    ${PROJECT_NAME}/game/physics.c
                    ${PROJECT_NAME}/game/player.c
                    ${PROJECT_NAME}/game/prerender.c
                    ${PROJECT_NAME}/game/random.c
                    ${PROJECT_NAME}/game/record.c
                    ${PROJECT_NAME}/game/render.c
                    ${PROJECT_NAME}/game/replay.c
                    ${PROJECT_NAME}/game/ropeedit.c
                    ${PROJECT_NAME}/game/setup.c
                    ${PROJECT_NAME}/game/socket.c
                    ${PROJECT_NAME}/game/sprite.c
                    ${PROJECT_NAME}/game/vsmode.c
                    ${PROJECT_NAME}/input/joystick.c
                    ${PROJECT_NAME}/input/keyboard.c
                    ${PROJECT_NAME}/input/mouse.c
                    ${PROJECT_NAME}/math/intersec.c
                    ${PROJECT_NAME}/math/vector.c
                    ${PROJECT_NAME}/menu/menu.c
                    ${PROJECT_NAME}/parser/parser.c
                    ${PROJECT_NAME}/physics/bond.c
                    ${PROJECT_NAME}/physics/object.c
                    ${PROJECT_NAME}/physics/particle.c
                    ${PROJECT_NAME}/sdl/endian.c
                    ${PROJECT_NAME}/sdl/event.c
                    ${PROJECT_NAME}/sdl/file.c
                    ${PROJECT_NAME}/sdl/video.c
                    ${PROJECT_NAME}/video/glfunc.c
                    ${PROJECT_NAME}/video/opengl.c
                    ${PROJECT_NAME}/video/text.c
                    ${PROJECT_NAME}/video/texture.c
                    ${PROJECT_NAME}/main.c)

add_executable(${TARGET_NAME} ${GISH_SOURCES})
target_link_libraries(${TARGET_NAME}   ${SDL2_LIBRARY}
                                        ${OPENAL_LIBRARY}
                                        ${OPENGL_LIBRARIES}
                                        ${Vorbis_LIBRARIES}
                                        ${PNG_LIBRARIES}
                                        m)
if(GLES)
    target_link_libraries(${TARGET_NAME} ${OPENGLES_LIBRARIES})
endif()

# Show final message when build done.
add_custom_target(finalMessage ALL
    ${CMAKE_COMMAND} -E cmake_echo_color --green "Output directory: ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}, executable: ${TARGET_NAME}."
    COMMENT "Final Message")
add_dependencies(finalMessage ${TARGET_NAME})
