LOCAL_PATH := $(call my-dir)

# Gish OpenGL Renderer
include $(CLEAR_VARS)

LOCAL_MODULE := Gish

SDL_PATH := ../SDL2
PNG_PATH := ../libpng
OPENAL_PATH := ../OpenAL
VORBIS_PATH := ../libvorbis/libvorbisidec
OGG_PATH := ../libvorbis/libogg
GL4ES_PATH := ../gl4es

DEFINES := -DANDROID_NDK -DGL4ES -DNO_PNG
LOCAL_CFLAGS += -O3 -ffast-math -fomit-frame-pointer $(DEFINES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
    $(LOCAL_PATH)/$(PNG_PATH) \
    $(LOCAL_PATH)/$(OPENAL_PATH)/include \
    $(LOCAL_PATH)/$(VORBIS_PATH) \
    $(LOCAL_PATH)/$(OGG_PATH) \
    $(LOCAL_PATH)/$(GL4ES_PATH) \
    $(LOCAL_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    main.c \
    android/a_utils.c \
    video/text.c \
    video/texture.c \
    video/opengl.c \
    video/glfunc.c \
    audio/audio.c \
    parser/parser.c \
    math/intersec.c \
    math/vector.c \
    input/mouse.c \
    input/joystick.c \
    input/keyboard.c \
    physics/particle.c \
    physics/object.c \
    physics/bond.c \
    menu/menu.c \
    sdl/video.c \
    sdl/file.c \
    sdl/endian.c \
    sdl/event.c \
    game/credits.c \
    game/gameaudio.c \
    game/custom.c \
    game/logic.c \
    game/record.c \
    game/ropeedit.c \
    game/gameobject.c \
    game/mainmenu.c \
    game/objedit.c \
    game/sprite.c \
    game/high.c \
    game/gamemenu.c \
    game/lighting.c \
    game/ai.c \
    game/game.c \
    game/music.c \
    game/vsmode.c \
    game/render.c \
    game/config.c \
    game/level.c \
    game/replay.c \
    game/debug.c \
    game/player.c \
    game/boss.c \
    game/socket.c \
    game/options.c \
    game/physics.c \
    game/objfunc.c \
    game/damage.c \
    game/gametexture.c \
    game/mappack.c \
    game/random.c \
    game/prerender.c \
    game/animation.c \
    game/block.c \
    game/setup.c \
    game/editor.c

LOCAL_SHARED_LIBRARIES := SDL2 openal
LOCAL_STATIC_LIBRARIES := vorbis GL

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)

# Gish OpenGL ES Renderer
include $(CLEAR_VARS)

LOCAL_MODULE := GishGLES

SDL_PATH := ../SDL2
PNG_PATH := ../libpng
OPENAL_PATH := ../OpenAL
VORBIS_PATH := ../libvorbis/libvorbisidec
OGG_PATH := ../libvorbis/libogg

DEFINES := -DANDROID_NDK -DGLES -DNO_PNG
LOCAL_CFLAGS += -O3 -ffast-math -fomit-frame-pointer $(DEFINES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
    $(LOCAL_PATH)/$(PNG_PATH) \
    $(LOCAL_PATH)/$(OPENAL_PATH)/include \
    $(LOCAL_PATH)/$(VORBIS_PATH) \
    $(LOCAL_PATH)/$(OGG_PATH) \
    $(LOCAL_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    main.c \
    android/a_utils.c \
    video/text.c \
    video/texture.c \
    video/opengl.c \
    video/glfunc.c \
    audio/audio.c \
    parser/parser.c \
    math/intersec.c \
    math/vector.c \
    input/mouse.c \
    input/joystick.c \
    input/keyboard.c \
    physics/particle.c \
    physics/object.c \
    physics/bond.c \
    menu/menu.c \
    sdl/video.c \
    sdl/file.c \
    sdl/endian.c \
    sdl/event.c \
    game/credits.c \
    game/gameaudio.c \
    game/custom.c \
    game/logic.c \
    game/record.c \
    game/ropeedit.c \
    game/gameobject.c \
    game/mainmenu.c \
    game/objedit.c \
    game/sprite.c \
    game/high.c \
    game/gamemenu.c \
    game/lighting.c \
    game/ai.c \
    game/game.c \
    game/music.c \
    game/vsmode.c \
    game/render.c \
    game/config.c \
    game/level.c \
    game/replay.c \
    game/debug.c \
    game/player.c \
    game/boss.c \
    game/socket.c \
    game/options.c \
    game/physics.c \
    game/objfunc.c \
    game/damage.c \
    game/gametexture.c \
    game/mappack.c \
    game/random.c \
    game/prerender.c \
    game/animation.c \
    game/block.c \
    game/setup.c \
    game/editor.c

LOCAL_SHARED_LIBRARIES := SDL2 openal
LOCAL_STATIC_LIBRARIES := vorbis

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
