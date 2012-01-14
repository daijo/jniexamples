#include <jni.h>
#include <stdio.h>
#include "MyNewString.h"

/* This code is OK */
jstring
MyNewString(JNIEnv *env, jchar *chars, jint len)
{
    static jclass stringClass = NULL;
    jmethodID cid;
    jcharArray elemArr;
    jstring result;

    if (stringClass == NULL) {
        jclass stringClassLocal =
            (*env)->FindClass(env, "java/lang/String");
        if (stringClassLocal == NULL) {
            return NULL; /* exception thrown */
        }
        /* Create a global reference */
        stringClass =
            (*env)->NewGlobalRef(env, stringClassLocal);

        /* The local reference is no longer useful */
        (*env)->DeleteLocalRef(env, stringClassLocal);

        /* Is the global reference created successfully? */
        if (stringClass == NULL) {
            return NULL; /* out of memory exception thrown */

        }
    }

    /* Get the method ID for the String(char[]) constructor */
    cid = (*env)->GetMethodID(env, stringClass,
                              "<init>", "([C)V");
    if (cid == NULL) {
        return NULL; /* exception thrown */
    }

    /* Create a char[] that holds the string characters */
    elemArr = (*env)->NewCharArray(env, len);
    if (elemArr == NULL) {
        return NULL; /* exception thrown */
    }
    (*env)->SetCharArrayRegion(env, elemArr, 0, len, chars);

    /* Construct a java.lang.String object */
    result = (*env)->NewObject(env, stringClass, cid, elemArr);

    /* Allow local ref to intermediate char[] to be freed */
    (*env)->DeleteLocalRef(env, elemArr);
    return result;
}

JNIEXPORT jstring JNICALL
Java_MyNewString_nativeMethod(JNIEnv *env, jclass cls)
{
    jchar str[] = {'a', 'b', 'c', 'd'};
    return MyNewString(env, str, sizeof(str) / sizeof(jchar));
}
