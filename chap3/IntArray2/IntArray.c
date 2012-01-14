#include <jni.h>
#include <stdio.h>
#include "IntArray.h"

JNIEXPORT jint JNICALL 
Java_IntArray_sumArray(JNIEnv *env, jobject obj, jintArray arr)
{
    jint *carr;
    jint i, sum = 0;
    carr = (*env)->GetIntArrayElements(env, arr, NULL);
    if (carr == NULL) {
        return 0; /* exception occurred */
    }

    for (i=0; i<10; i++) {
        sum += carr[i];
    }

    (*env)->ReleaseIntArrayElements(env, arr, carr, 0);
    return sum;
}
