#include <jni.h>
#include <stdio.h>
#include "InstanceFieldAccess.h"  

JNIEXPORT void JNICALL 
Java_InstanceFieldAccess_accessField(JNIEnv *env, jobject obj)
{
    static jfieldID fid_s = NULL; /* cached field ID for s */

    jclass cls = (*env)->GetObjectClass(env, obj);
    jstring jstr;
    const char *str;

    if (fid_s == NULL) {
        fid_s = (*env)->GetFieldID(env, cls, "s", 
                                   "Ljava/lang/String;");
        if (fid_s == 0) {
            return; /* exception already thrown */
        }
    }

    printf("In C:\n");

    jstr = (*env)->GetObjectField(env, obj, fid_s);
    str = (*env)->GetStringUTFChars(env, jstr, 0);
    if (str == NULL) {
        return; /* out of memory */
    }
    printf("  c.s = \"%s\"\n", str);
    (*env)->ReleaseStringUTFChars(env, jstr, str);

    jstr = (*env)->NewStringUTF(env, "123");
    (*env)->SetObjectField(env, obj, fid_s, jstr);
}
