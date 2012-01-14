#include <windows.h>
#include <jni.h>
#include <stdio.h>
#include "C.h"
#include "Win32.h"

JNIEXPORT jint JNICALL 
  Java_C_atol(JNIEnv *env, jclass cls, jstring str)
{
    const char *cstr = env->GetStringUTFChars(str, 0);
    if (cstr == NULL) {
        return 0; /* out of memory */
    }
    int result = atol(cstr);
    env->ReleaseStringUTFChars(str, cstr);
    return result;
}


JNIEXPORT jint JNICALL Java_Win32_CreateFile(
        JNIEnv *env,
        jclass cls,
        jstring  fileName,         // file name
        jint desiredAccess,        // access (read-write) mode 
        jint shareMode,            // share mode 
        jintArray secAttrs,        // security attributes 
        jint creationDistribution, // how to create 
        jint flagsAndAttributes,   // file attributes 
        jint templateFile)         // file with attr. to copy
{
    jint result = 0;
    jint *cSecAttrs = NULL;
    if (secAttrs) {
        cSecAttrs = env->GetIntArrayElements(secAttrs, 0);
        if (cSecAttrs == NULL) {
            return 0; /* out of memory */
        }
    }
    const char *cFileName = env->GetStringUTFChars(fileName, NULL);
    if (cFileName) {
        /* call the real Win32 function */
        result = (jint)CreateFile(cFileName,
                            desiredAccess,
                            shareMode,
                            (SECURITY_ATTRIBUTES *)cSecAttrs,
                            creationDistribution,
                            flagsAndAttributes,
                            (HANDLE)templateFile);
        //free(cFileName);
    }
    /* else fall through, out of memory exception thrown */
    if (secAttrs) {
        env->ReleaseIntArrayElements(secAttrs, cSecAttrs, 0);
    }
    return result;
}
