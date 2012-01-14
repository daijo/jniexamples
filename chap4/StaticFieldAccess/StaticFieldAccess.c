#include <jni.h>
#include <stdio.h>
#include "StaticFieldAccess.h"

JNIEXPORT void JNICALL 
Java_StaticFieldAccess_accessField(JNIEnv *env, jobject obj)
{
    jfieldID fid;   /* store the field ID */
    jint si;

    /* Get a reference to objÆs class */
    jclass cls = (*env)->GetObjectClass(env, obj);

    printf("In C:\n");

    /* Look for the static field si in cls */
    fid = (*env)->GetStaticFieldID(env, cls, "si", "I");
    if (fid == 0) {
        return; /* field not found */
    }
    /* Access the static field si */
    si = (*env)->GetStaticIntField(env, cls, fid);
    printf("  StaticFieldAccess.si = %d\n", si);
    (*env)->SetStaticIntField(env, cls, fid, 200);
}
