#include <string.h>
#include <jni.h>
#include <android/log.h>

#define MP3DEC_TAG    "[MP3DEC-JNI]"

jstring Java_com_wb_mp3dec_Mp3Dec_Version( JNIEnv* env, jobject thiz ) {
    __android_log_write(ANDROID_LOG_INFO, MP3DEC_TAG, "-- Mp3Decoder Beta 1.0 --");
    return (*env)->NewStringUTF(env, "Mp3Decoder Beta 1.0");
}
