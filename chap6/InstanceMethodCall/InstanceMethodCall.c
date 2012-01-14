#include <jni.h>
#include <stdio.h>
#include "InstanceMethodCall.h"

jvalue
JNU_CallMethodByName(JNIEnv *env,
                     jboolean *hasException,
                     jobject obj, 
                     const char *name,
                     const char *descriptor,
                     ...)
{
    va_list args;
    jclass clazz;
    jmethodID mid;
    jvalue result;

    if ((*env)->EnsureLocalCapacity(env, 2) == JNI_OK) {
        clazz = (*env)->GetObjectClass(env, obj);
        mid = (*env)->GetMethodID(env, clazz, name, descriptor);
        if (mid) {
            const char *p = descriptor;
            /* skip over argument types to find out the 
             * return type */
            while (*p != ')') p++;
            /* skip ')' */
            p++;
            va_start(args, descriptor);
            switch (*p) {
            case 'V':
                (*env)->CallVoidMethodV(env, obj, mid, args);
                break;
            case '[':
            case 'L':
                result.l = (*env)->CallObjectMethodV(
                                       env, obj, mid, args);
                break;
            case 'Z':
                result.z = (*env)->CallBooleanMethodV(
                                       env, obj, mid, args);
                break;
            case 'B':
                result.b = (*env)->CallByteMethodV(
                                       env, obj, mid, args);
                break;
            case 'C':
                result.c = (*env)->CallCharMethodV(
                                       env, obj, mid, args);
                break;
            case 'S':
                result.s = (*env)->CallShortMethodV(
                                       env, obj, mid, args);
                break;
            case 'I':
                result.i = (*env)->CallIntMethodV(
                                       env, obj, mid, args);
                break;
            case 'J':
                result.j = (*env)->CallLongMethodV(
                                       env, obj, mid, args);
                break;
            case 'F':
                result.f = (*env)->CallFloatMethodV(
                                       env, obj, mid, args);
                break;
            case 'D':
                result.d = (*env)->CallDoubleMethodV(
                                       env, obj, mid, args);
                break;
            default:
                (*env)->FatalError(env, "illegal descriptor");
            }
            va_end(args);
        }
        (*env)->DeleteLocalRef(env, clazz);
    }
    if (hasException) {
        *hasException = (*env)->ExceptionCheck(env);
    }
    return result;
}

JNIEXPORT void JNICALL 
Java_InstanceMethodCall_nativeMethod(JNIEnv *env, jobject obj)
{
    printf("In C\n");
    JNU_CallMethodByName(env, NULL, obj, "callback", "()V");
}
