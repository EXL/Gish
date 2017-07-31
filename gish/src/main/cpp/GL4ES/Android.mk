LOCAL_PATH := $(call my-dir)

###########################
#
# GL static library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := GL4ES

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES) -DBCMHOST

LOCAL_SRC_FILES := \
	src/gl/array.c \
	src/gl/blend.c \
	src/gl/blit.c \
	src/gl/buffers.c \
	src/gl/debug.c \
	src/gl/decompress.c \
	src/gl/directstate.c \
	src/gl/eval.c \
	src/gl/fog.c \
	src/gl/framebuffers.c \
	src/gl/getter.c \
	src/gl/gl.c \
	src/gl/hint.c \
	src/gl/init.c \
	src/gl/light.c \
	src/gl/line.c \
	src/gl/list.c \
	src/gl/loader.c \
	src/gl/matrix.c \
	src/gl/matvec.c \
	src/gl/pixel.c \
	src/gl/planes.c \
	src/gl/queries.c \
	src/gl/raster.c \
	src/gl/render.c \
	src/gl/shader.c \
	src/gl/stack.c \
	src/gl/texenv.c \
	src/gl/texgen.c \
	src/gl/texture.c \
	src/gl/uniform.c \
	src/gl/vertexattrib.c \
	src/gl/wrap/gl.c \
	src/gl/wrap/gles.c \
	src/gl/wrap/glstub.c \
	src/gl/math/eval.c \
	src/glx/hardext.c \
	src/glx/glx.c \
	src/glx/lookup.c \
	src/glx/streaming.c \

LOCAL_CFLAGS += -std=c99 -funwind-tables -O3 -ffast-math -fomit-frame-pointer -DBCMHOST -fvisibility=hidden -include include/android_debug.h
LOCAL_CFLAGS += -DNO_INIT_CONSTRUCTOR

include $(BUILD_STATIC_LIBRARY)
