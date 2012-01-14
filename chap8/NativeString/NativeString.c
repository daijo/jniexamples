#include <jni.h>
#include <stdio.h>
#include "NativeString.h"  

JavaVM *cached_jvm;
jclass Class_C;
jmethodID MID_C_g;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    JNIEnv *env;
    jclass cls;
    cached_jvm = jvm;  /* cache the JavaVM pointer */
    if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2)) {
        return JNI_ERR;
    }
    cls = (*env)->FindClass(env, "java/lang/String");
    if (cls == NULL) {
        return JNI_ERR;
    }
    /* Use weak global ref to allow C class to be unloaded */
    Class_C = (*env)->NewWeakGlobalRef(env, cls);
    if (Class_C == NULL) {
        return JNI_ERR;
    }
    /* Compute and cache the method ID */
    MID_C_g = (*env)->GetMethodID(env, cls, "getBytes", "()[B");
    if (MID_C_g == NULL) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_2;
}

JNIEXPORT void JNICALL 
JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    JNIEnv *env;
    if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2)) {
        return;
    }
    (*env)->DeleteWeakGlobalRef(env, Class_C);
    return;
}

jclass Class_java_lang_String;
jmethodID MID_String_init;
jmethodID MID_String_getBytes;

void initIDs(JNIEnv *env)
{
  Class_java_lang_String = (*env)->FindClass(env, "java/lang/String");
  MID_String_init = (*env)->GetMethodID(env, Class_java_lang_String,
				"<init>", "([B)V");
  MID_String_getBytes = (*env)->GetMethodID(env, Class_java_lang_String,
				"getBytes", "()[B");
}

jstring JNU_NewStringNative(JNIEnv *env, const char *str)
{
    jstring result;
    jbyteArray bytes = 0;
    int len;
    if ((*env)->EnsureLocalCapacity(env, 2) < 0) {
        return NULL; /* out of memory error */
    }
    len = strlen(str);
    bytes = (*env)->NewByteArray(env, len);
    if (bytes != NULL) {
        (*env)->SetByteArrayRegion(env, bytes, 0, len,
                                   (jbyte *)str);
        result = (*env)->NewObject(env, Class_java_lang_String,
                                   MID_String_init, bytes);
        (*env)->DeleteLocalRef(env, bytes);
        return result;
    } /* else fall through */
    return NULL;
}

char *JNU_GetStringNativeChars(JNIEnv *env, jstring jstr)
{
    jbyteArray bytes = 0;
    jthrowable exc;
    char *result = 0;
    if ((*env)->EnsureLocalCapacity(env, 2) < 0) {
        return 0; /* out of memory error */
    }
    bytes = (*env)->CallObjectMethod(env, jstr,
                                     MID_String_getBytes);
    exc = (*env)->ExceptionOccurred(env);
    if (!exc) {
        jint len = (*env)->GetArrayLength(env, bytes);
        result = (char *)malloc(len + 1);
        if (result == 0) {
            JNU_ThrowByName(env, "java/lang/OutOfMemoryError",
                            0);
            (*env)->DeleteLocalRef(env, bytes);
            return 0;
        }
        (*env)->GetByteArrayRegion(env, bytes, 0, len,
                                   (jbyte *)result);
        result[len] = 0; /* NULL-terminate */
    } else {
        (*env)->DeleteLocalRef(env, exc);
    }
    (*env)->DeleteLocalRef(env, bytes);
    return result;
}

JNIEXPORT jstring JNICALL
Java_NativeString_nativeMethod(JNIEnv *env, jclass cls, jstring jstr)
{
  initIDs(env);
  {
    char *str = JNU_GetStringNativeChars(env, jstr);
    return JNU_NewStringNative(env, str);
  }
}
