#include <stdio.h>
#include <jni.h>
#include "ThrowByName.h"

void
JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg)
{
    jclass cls = (*env)->FindClass(env, name);
    /* If cls is NULL, an exception has already been thrown */
    if (cls != NULL) {
        (*env)->ThrowNew(env, cls, msg);
    }
    /* free the local ref */
    (*env)->DeleteLocalRef(env, cls);
}

JNIEXPORT void JNICALL 
Java_ThrowByName_doit(JNIEnv *env, jobject obj)
{
    JNU_ThrowByName(env, "java/lang/InternalError", "XXXX");
}
