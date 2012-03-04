#include <string.h>
#include <jni.h>

jstring Java_com_wb_mp3dec_Mp3Dec_Version( JNIEnv* env, jobject thiz ) {
    return (*env)->NewStringUTF(env, "Mp3Decoder Beta 1.0");
}
