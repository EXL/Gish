LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libvorbis

OGG_LIBRARY_PATH := libogg
VORBIS_LIBRARY_PATH := libvorbisidec

OPTIM := -O3 -ffast-math -fomit-frame-pointer
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(OGG_LIBRARY_PATH)/include $(LOCAL_PATH)/$(VORBIS_LIBRARY_PATH)
LOCAL_CFLAGS += -DOGG_MUSIC -DOGG_USE_TREMOR -DOGG_HEADER="<ivorbisfile.h>" $(OPTIM)
ifeq ($(TARGET_ARCH_ABI),armeabi)
LOCAL_CFLAGS += -D_ARM_ASSEM_
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D_ARM_ASSEM_
endif
LOCAL_SRC_FILES += \
	$(VORBIS_LIBRARY_PATH)/mdct.c \
	$(VORBIS_LIBRARY_PATH)/block.c \
	$(VORBIS_LIBRARY_PATH)/window.c \
	$(VORBIS_LIBRARY_PATH)/synthesis.c \
	$(VORBIS_LIBRARY_PATH)/info.c \
	$(VORBIS_LIBRARY_PATH)/floor1.c \
	$(VORBIS_LIBRARY_PATH)/floor0.c \
	$(VORBIS_LIBRARY_PATH)/vorbisfile.c \
	$(VORBIS_LIBRARY_PATH)/res012.c \
	$(VORBIS_LIBRARY_PATH)/mapping0.c \
	$(VORBIS_LIBRARY_PATH)/registry.c \
	$(VORBIS_LIBRARY_PATH)/codebook.c \
	$(VORBIS_LIBRARY_PATH)/sharedbook.c \
	$(OGG_LIBRARY_PATH)/src/framing.c \
	$(OGG_LIBRARY_PATH)/src/bitwise.c

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

LOCAL_ARM_MODE := arm

include $(BUILD_STATIC_LIBRARY)
