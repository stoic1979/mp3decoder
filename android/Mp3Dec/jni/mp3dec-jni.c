/******************************************************************************
*
* JNI wrapper for calling the C functions of the decoder
*
******************************************************************************/

#include <string.h>
#include <jni.h>
#include <android/log.h>

#include <stdio.h>
#include <wchar.h>
#include "app.h"

#define MP3DEC_TAG    "[MP3DEC-JNI]"

jstring Java_com_wb_mp3dec_Mp3Dec_Version( JNIEnv* env, jobject thiz ) {
    __android_log_write(ANDROID_LOG_INFO, MP3DEC_TAG, "-- Mp3Decoder Beta 1.0 --");
    return (*env)->NewStringUTF(env, "Mp3Decoder Beta 1.0");
}

void Java_com_wb_mp3dec_Mp3Dec_Decode( JNIEnv* env, jobject thiz, jstring srcpath, jstring despath) {

    /* compose native strings */
    const char* native_srcpath = (*env)->GetStringUTFChars(env, srcpath, 0);
    const char* native_despath = (*env)->GetStringUTFChars(env, despath, 0);

    /* msg */
    char msg[1024];
    strcpy(msg, " -- decoding: ");
    strcat(msg, native_srcpath);
    strcat(msg, " --> ");
    strcat(msg, native_despath);

    __android_log_write(ANDROID_LOG_INFO, MP3DEC_TAG, msg);
    
    if(decoder_lib_main(native_srcpath, native_despath) < 0) {
        __android_log_write(ANDROID_LOG_INFO, MP3DEC_TAG, "-- ERROR WHILE DECODING --");
    }

    /* release allocated string */
    (*env)->ReleaseStringUTFChars(env, srcpath, native_srcpath);
    (*env)->ReleaseStringUTFChars(env, despath, native_despath);
}
