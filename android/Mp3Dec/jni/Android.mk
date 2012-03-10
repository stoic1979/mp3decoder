################################################################
#
# Mp3Decoder Android Makefile wrapper
#
################################################################


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS    := -DHAVE_MPGLIB

LOCAL_MODULE := wblame
LOCAL_SRC_FILES := mp3dec-jni.c \
    bitstream.c \
    fft.c \
    id3tag.c \
    mpglib_interface.c \
    presets.c \
    quantize.c \
    reservoir.c \
    tables.c \
    util.c \
    VbrTag.c \
    encoder.c \
    gain_analysis.c \
    lame.c \
    newmdct.c \
    psymodel.c \
    quantize_pvt.c \
    set_get.c \
    takehiro.c \
    vbrquantize.c \
    version.c \
    app.c \
    app_utils.c \
    app_fft.c \
    process_fft.c \
    common.c dct64_i386.c decode_i386.c interface.c layer1.c layer2.c layer3.c tabinit.c

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
