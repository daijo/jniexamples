#include <jni.h>
#include <stdio.h>
#include "MyNewString.h"  

jstring
MyNewString(JNIEnv *env, jchar *chars, jint len)
{
    jclass stringClass;
    jcharArray elemArr;
    static jmethodID cid = NULL;
    jstring result;

    stringClass = (*env)->FindClass(env, "java/lang/String");
    if (stringClass == NULL) {
        return NULL; /* exception thrown */
    }

    /* Note that cid is a static variable */
    if (cid == NULL) {
        /* Get the method ID for the String constructor */
        cid = (*env)->GetMethodID(env, stringClass,
                                  "<init>", "([C)V");
        if (cid == NULL) {
            return NULL; /* exception thrown */
        }
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
