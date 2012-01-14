/*
 * %W% %E%
 * 
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

/*
 * JNI native methods supporting the infrastructure for shared
 * dispatchers.  Includes native methods for classes CPointer, CFunction, and
 * CMalloc.
 */

/*
 * The implementation here is more general than what is described
 * in the JNI book in handling return types other than "int". You
 * can, however, define the JNI_BOOK constant to make the
 * Java_CFunction_callInt implementation exactly the same as what
 * is described in the JNI book.
 */

#ifdef SOLARIS2
#include <dlfcn.h>
#define LOAD_LIBRARY(name) dlopen(name, RTLD_LAZY)
#define FIND_ENTRY(lib, name) dlsym(lib, name)
#endif

#ifdef WIN32
#include <windows.h>
#define LOAD_LIBRARY(name) LoadLibrary(name)
#define FIND_ENTRY(lib, name) GetProcAddress(lib, name)
#endif

#include <stdlib.h>
#include <string.h>

#include <jni.h>

#include "CPointer.h"
#include "CFunction.h"
#include "CMalloc.h"

/* Global references to frequently used classes and objects */
static jclass Class_String;
static jclass Class_Integer;
static jclass Class_Float;
static jclass Class_Double;
static jclass Class_CPointer;

/* Cached field and method IDs */
static jmethodID MID_String_getBytes;
static jmethodID MID_String_init;

static jfieldID FID_Integer_value;
static jfieldID FID_Float_value;
static jfieldID FID_Double_value;
static jfieldID FID_CPointer_peer;
static jfieldID FID_CFunction_conv;

/* Forward declarations */
static void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg);
static char * JNU_GetStringNativeChars(JNIEnv *env, jstring jstr);
static jstring JNU_NewStringNative(JNIEnv *env, const char *str);
static jobject makeCPointer(JNIEnv *env, void *p);


/********************************************************************/
/*		     Native methods of class CFunction		    */
/********************************************************************/

/* These are the set of types CFunction can handle now */
typedef enum {
    TY_CPTR = 0,
    TY_INTEGER,
    TY_FLOAT,
    TY_DOUBLE,
    TY_DOUBLE2,
    TY_STRING
} ty_t;

/* represent a machine word */
typedef union {
    jint i;
    jfloat f;
    void *p;
} word_t;

/* A CPU-dependent assembly routine that passes the arguments to C
 * stack and invoke the function.
 */
extern "C" void 
asm_dispatch(void *func,
	     int nwords,
	     char *args_types,
	     word_t *args,
	     ty_t res_type,
	     word_t *resP,
	     int conv);

/* invoke the real native function */
static void
dispatch(JNIEnv *env,
	 jobject self,
	 jobjectArray arr,
	 ty_t res_ty,
	 jvalue *resP)
{
#define MAX_NARGS 32
    int i, nargs, nwords;
    void *func;
    char argTypes[MAX_NARGS];
    word_t c_args[MAX_NARGS * 2];
    int conv;

    nargs = env->GetArrayLength(arr);
    if (nargs > MAX_NARGS) {
        JNU_ThrowByName(env, "java/lang/IllegalArgumentException",
		    "too many arguments");
	return;
    }
    func = (void *)env->GetLongField(self, FID_CPointer_peer);

    for (nwords = 0, i = 0; i < nargs; i++) {
        jobject arg = env->GetObjectArrayElement(arr, i);
	if (arg == NULL) {
	    c_args[nwords].p = NULL;
	    argTypes[nwords++] = TY_CPTR;
	} else if (env->IsInstanceOf(arg, Class_Integer)) {
	    c_args[nwords].i =
	        env->GetIntField(arg, FID_Integer_value);
	    argTypes[nwords++] = TY_INTEGER;
	} else if (env->IsInstanceOf(arg, Class_CPointer)) {
	    c_args[nwords].p = 
	        (void *)env->GetLongField(arg, FID_CPointer_peer);
	    argTypes[nwords++] = TY_CPTR;
	} else if (env->IsInstanceOf(arg, Class_String)) {
	    if ((c_args[nwords].p = JNU_GetStringNativeChars(env, (jstring)arg)) == 0) {
	        goto cleanup;
	    }
	    argTypes[nwords++] = TY_STRING;
	} else if (env->IsInstanceOf(arg, Class_Float)) {
	    c_args[nwords].f =
	        env->GetFloatField(arg, FID_Float_value);
	    argTypes[nwords++] = TY_FLOAT;
	} else if (env->IsInstanceOf(arg, Class_Double)) {
	    *(jdouble *)(c_args + nwords) = 
	        env->GetDoubleField(arg, FID_Double_value);
	    argTypes[nwords] = TY_DOUBLE;
	    /* harmless with 64-bit machines*/
	    argTypes[nwords + 1] = TY_DOUBLE2;
	    /* make sure things work on 64-bit machines */
	    nwords += sizeof(jdouble) / sizeof(word_t);
	} else {
	    JNU_ThrowByName(env, "java/lang/IllegalArgumentException",
			"unrecognized argument type");
	    goto cleanup;
	}
	env->DeleteLocalRef(arg);
    }

    conv = env->GetIntField(self, FID_CFunction_conv);
    asm_dispatch(func, nwords, argTypes, c_args, res_ty, (word_t *)resP, conv);

cleanup:
    for (i = 0; i < nwords; i++) {
        if (argTypes[i] == TY_STRING) {
	    free(c_args[i].p);
	}
    }
    return;
}

/*
 * Class:     CFunction
 * Method:    initIDs
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_CFunction_initIDs(JNIEnv *env, jclass cls)
{
    FID_CFunction_conv = env->GetFieldID(cls, "conv", "I");
}

/*
 * Class:     CFunction
 * Method:    callCPointer
 * Signature: ([Ljava/lang/Object;)LCPointer;
 */
JNIEXPORT jobject JNICALL 
Java_CFunction_callCPointer(JNIEnv *env, jobject self, jobjectArray arr)
{
    jvalue result;
    dispatch(env, self, arr, TY_CPTR, &result);
    if (env->ExceptionOccurred()) {
        return NULL;
    }
    return makeCPointer(env, (void *)result.j);
}

/*
 * Class:     CFunction
 * Method:    callDouble
 * Signature: ([Ljava/lang/Object;)D
 */
JNIEXPORT jdouble JNICALL 
Java_CFunction_callDouble(JNIEnv *env, jobject self, jobjectArray arr)
{
    jvalue result;
    dispatch(env, self, arr, TY_DOUBLE, &result);
    return result.d;
}

/*
 * Class:     CFunction
 * Method:    callFloat
 * Signature: ([Ljava/lang/Object;)F
 */
JNIEXPORT jfloat JNICALL
Java_CFunction_callFloat(JNIEnv *env, jobject self, jobjectArray arr)
{
    jvalue result;
    dispatch(env, self, arr, TY_FLOAT, &result);
    return result.f;
}

#ifdef JNI_BOOK

extern "C" int asm_dispatch_int(void *func,
                     int nwords,
                     word_t *args,
                     int conv);     

JNIEXPORT jint JNICALL Java_CFunction_callInt
  (JNIEnv *env, jobject self, jobjectArray arr)
{
#define MAX_NARGS 32
    jint ires;
    int nargs, nwords;
    void *func;
    jboolean is_string[MAX_NARGS];
    word_t args[MAX_NARGS];
    int conv;

    nargs = env->GetArrayLength(arr);
    if (nargs > MAX_NARGS) {
        JNU_ThrowByName(env, 
                    "java/lang/IllegalArgumentException",
                    "too many arguments");
        return 0;
    }

    // convert arguments
    for (nwords = 0; nwords < nargs; nwords++) {
        is_string[nwords] = JNI_FALSE;
        jobject arg = env->GetObjectArrayElement(arr, nwords);

        if (arg == NULL) {
            args[nwords].p = NULL;
        } else if (env->IsInstanceOf(arg, Class_Integer)) {
            args[nwords].i =
                env->GetIntField(arg, FID_Integer_value);
        } else if (env->IsInstanceOf(arg, Class_Float)) {
            args[nwords].f =
                env->GetFloatField(arg, FID_Float_value);
        } else if (env->IsInstanceOf(arg, Class_CPointer)) {
            args[nwords].p = (void *)
                env->GetLongField(arg, FID_CPointer_peer);
        } else if (env->IsInstanceOf(arg, Class_String)) {
            char * cstr = JNU_GetStringNativeChars(env, (jstring)arg);
            if ((args[nwords].p = cstr) == NULL) {
                goto cleanup; // error thrown
            }
            is_string[nwords] = JNI_TRUE;
        } else {
            JNU_ThrowByName(env,
                "java/lang/IllegalArgumentException",
                "unrecognized argument type");
            goto cleanup;
        }
        env->DeleteLocalRef(arg);
    }

    func = (void *)env->GetLongField(self, FID_CPointer_peer);
    conv = env->GetIntField(self, FID_CFunction_conv);

    // now transfer control to func.
    ires = asm_dispatch_int(func, nwords, args, conv);

cleanup:
    // free all the native strings we have created
    for (int i = 0; i < nwords; i++) {
        if (is_string[i]) {
            free(args[i].p);
        }
    }
    return ires;
}

#else /* JNI_BOOK */

JNIEXPORT jint JNICALL
Java_CFunction_callInt(JNIEnv *env, jobject self, jobjectArray arr)
{
    jvalue result;
    dispatch(env, self, arr, TY_INTEGER, &result);
    return result.i;
}
#endif /* JNI_BOOK */

/*
 * Class:     CFunction
 * Method:    callVoid
 * Signature: ([Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL
Java_CFunction_callVoid(JNIEnv *env, jobject self, jobjectArray arr)
{
    jvalue result;
    dispatch(env, self, arr, TY_INTEGER, &result);
}

/*
 * Class:     CFunction
 * Method:    find
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_CFunction_find
  (JNIEnv *env, jobject self, jstring lib, jstring fun)
{
    void *handle;
    void *func;
    char *libname;
    char *funname;

    if ((libname = JNU_GetStringNativeChars(env, lib))) {
        if ((funname = JNU_GetStringNativeChars(env, fun))) {
            if ((handle = (void *)LOAD_LIBRARY(libname))) {
                if (!(func = (void *)FIND_ENTRY(handle, funname))) {
                    JNU_ThrowByName(env, 
                        "java/lang/UnsatisfiedLinkError",
                        funname);
                }
            } else {
                JNU_ThrowByName(env, 
                        "java/lang/UnsatisfiedLinkError",
                        libname);
            }
            free(funname);
        }
        free(libname);
    }
    return (jlong)func;
}

/********************************************************************/
/*		     Native methods of class CPointer		    */
/********************************************************************/

/*
 * Class:     CPointer
 * Method:    initIDs
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_CPointer_initIDs(JNIEnv *env, jclass cls)
{
    Class_String = env->FindClass("java/lang/String");
    if (Class_String == NULL) return 0;
    Class_String = (jclass)env->NewGlobalRef(Class_String);
    if (Class_String == NULL) return 0;

    Class_Integer = env->FindClass("java/lang/Integer");
    if (Class_Integer == NULL) return 0;
    Class_Integer = (jclass)env->NewGlobalRef(Class_Integer);
    if (Class_Integer == NULL) return 0;

    Class_Float = env->FindClass("java/lang/Float");
    if (Class_Float == NULL) return 0;
    Class_Float = (jclass)env->NewGlobalRef(Class_Float);
    if (Class_Float == NULL) return 0;

    Class_Double = env->FindClass("java/lang/Double");
    if (Class_Double == NULL) return 0;
    Class_Double = (jclass)env->NewGlobalRef(Class_Double);
    if (Class_Double == NULL) return 0;

    Class_CPointer = (jclass)env->NewGlobalRef(cls);
    if (Class_CPointer == NULL) return 0;

    MID_String_getBytes = 
        env->GetMethodID(Class_String, "getBytes", "()[B");
    if (MID_String_getBytes == NULL) return 0;
    MID_String_init = env->GetMethodID(Class_String, "<init>", "([B)V");
    if (MID_String_init == NULL) return 0;

    FID_Integer_value = env->GetFieldID(Class_Integer, "value", "I");
    if (FID_Integer_value == NULL) return 0;

    FID_Float_value = env->GetFieldID(Class_Float, "value", "F");
    if (FID_Float_value == NULL) return 0;

    FID_Double_value = env->GetFieldID(Class_Double, "value", "D");
    if (FID_Double_value == NULL) return 0;

    FID_CPointer_peer = env->GetFieldID(Class_CPointer, "peer", "J");
    return sizeof(void *);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[BII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3BII
  (JNIEnv *env, jobject self, jint boff, jbyteArray arr, jint off, jint n)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    env->GetByteArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[CII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3CII
  (JNIEnv *env, jobject self, jint boff, jcharArray arr, jint off, jint n)
{
    jchar *peer = (jchar *)env->GetLongField(self, FID_CPointer_peer);
    env->GetCharArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[DII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3DII
  (JNIEnv *env, jobject self, jint boff, jdoubleArray arr, jint off, jint n)
{
    jdouble *peer = (jdouble *)
        env->GetLongField(self, FID_CPointer_peer);
    env->GetDoubleArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[FII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3FII
  (JNIEnv *env, jobject self, jint boff, jfloatArray arr, jint off, jint n)
{
    jfloat *peer = (jfloat *)
        env->GetLongField(self, FID_CPointer_peer);
    env->GetFloatArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[III)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3III
  (JNIEnv *env, jobject self, jint boff, jintArray arr, jint off, jint n)
{
    jint *peer = (jint *)env->GetLongField(self, FID_CPointer_peer);
    env->GetIntArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[JII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3JII
  (JNIEnv *env, jobject self, jint boff, jlongArray arr, jint off, jint n)
{
    jlong *peer = (jlong *)env->GetLongField(self, FID_CPointer_peer);
    env->GetLongArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyIn
 * Signature: (I[SII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyIn__I_3SII
  (JNIEnv *env, jobject self, jint boff, jshortArray arr, jint off, jint n)
{
    jshort *peer = (jshort *)
        env->GetLongField(self, FID_CPointer_peer);
    env->GetShortArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[BII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3BII
  (JNIEnv *env, jobject self, jint boff, jbyteArray arr, jint off, jint n)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    env->SetByteArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[CII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3CII
  (JNIEnv *env, jobject self, jint boff, jcharArray arr, jint off, jint n)
{
    jchar *peer = (jchar *)env->GetLongField(self, FID_CPointer_peer);
    env->SetCharArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[DII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3DII
  (JNIEnv *env, jobject self, jint boff, jdoubleArray arr, jint off, jint n)
{
    jdouble *peer = (jdouble *)
        env->GetLongField(self, FID_CPointer_peer);
    env->SetDoubleArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[FII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3FII
  (JNIEnv *env, jobject self, jint boff, jfloatArray arr, jint off, jint n)
{
    jfloat *peer = (jfloat *)
        env->GetLongField(self, FID_CPointer_peer);
    env->SetFloatArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[III)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3III
  (JNIEnv *env, jobject self, jint boff, jintArray arr, jint off, jint n)
{
    jint *peer = (jint *)env->GetLongField(self, FID_CPointer_peer);
    env->SetIntArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[JII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3JII
  (JNIEnv *env, jobject self, jint boff, jlongArray arr, jint off, jint n)
{
    jlong *peer = (jlong *)env->GetLongField(self, FID_CPointer_peer);
    env->SetLongArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    copyOut
 * Signature: (I[SII)V
 */
JNIEXPORT void JNICALL Java_CPointer_copyOut__I_3SII
  (JNIEnv *env, jobject self, jint boff, jshortArray arr, jint off, jint n)
{
    jshort *peer = (jshort *)
        env->GetLongField(self, FID_CPointer_peer);
    env->SetShortArrayRegion(arr, off, n, peer + boff);
}

/*
 * Class:     CPointer
 * Method:    getByte
 * Signature: (I)B
 */
JNIEXPORT jbyte JNICALL Java_CPointer_getByte
  (JNIEnv *env, jobject self, jint index)
{
    jbyte res;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&res, peer + index, sizeof(res));
    return res;
}

/*
 * Class:     CPointer
 * Method:    getCPointer
 * Signature: (I)LCPointer;
 */
JNIEXPORT jobject JNICALL Java_CPointer_getCPointer
  (JNIEnv *env, jobject self, jint index)
{
    void *ptr;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&ptr, peer + index, sizeof(ptr));
    return makeCPointer(env, ptr);
}

/*
 * Class:     CPointer
 * Method:    getDouble
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_CPointer_getDouble
  (JNIEnv *env, jobject self, jint index)
{
    jdouble res;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&res, peer + index, sizeof(res));
    return res;
}

/*
 * Class:     CPointer
 * Method:    getFloat
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_CPointer_getFloat
  (JNIEnv *env, jobject self, jint index)
{
    jfloat res;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&res, peer + index, sizeof(res));
    return res;
}

/*
 * Class:     CPointer
 * Method:    getInt
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_CPointer_getInt
  (JNIEnv *env, jobject self, jint index)
{
    jint res;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&res, peer + index, sizeof(res));
    return res;
}

/*
 * Class:     CPointer
 * Method:    getLong
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_CPointer_getLong
  (JNIEnv *env, jobject self, jint index)
{
    jlong res;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&res, peer + index, sizeof(res));
    return res;
}

/*
 * Class:     CPointer
 * Method:    getShort
 * Signature: (I)S
 */
JNIEXPORT jshort JNICALL Java_CPointer_getShort
  (JNIEnv *env, jobject self, jint index)
{
    jshort res;
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(&res, peer + index, sizeof(res));
    return res;
}

/*
 * Class:     CPointer
 * Method:    getString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_CPointer_getString
  (JNIEnv *env, jobject self, jint index)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    return JNU_NewStringNative(env, (const char *)peer + index);
}

/*
 * Class:     CPointer
 * Method:    setByte
 * Signature: (IB)V
 */
JNIEXPORT void JNICALL Java_CPointer_setByte
  (JNIEnv *env, jobject self, jint index, jbyte value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &value, sizeof(value));
}

/*
 * Class:     CPointer
 * Method:    setCPointer
 * Signature: (ILCPointer;)V
 */
JNIEXPORT void JNICALL Java_CPointer_setCPointer
  (JNIEnv *env, jobject self, jint index, jobject cptr)
{
    void *ptr = (void *)env->GetLongField(cptr, FID_CPointer_peer);
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &ptr, sizeof(ptr));
}

/*
 * Class:     CPointer
 * Method:    setDouble
 * Signature: (ID)V
 */
JNIEXPORT void JNICALL Java_CPointer_setDouble
  (JNIEnv *env, jobject self, jint index, jdouble value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &value, sizeof(value));
}

/*
 * Class:     CPointer
 * Method:    setFloat
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_CPointer_setFloat
  (JNIEnv *env, jobject self, jint index, jfloat value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &value, sizeof(value));
}

/*
 * Class:     CPointer
 * Method:    setInt
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_CPointer_setInt
  (JNIEnv *env, jobject self, jint index, jint value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &value, sizeof(value));
}

/*
 * Class:     CPointer
 * Method:    setLong
 * Signature: (IJ)V
 */
JNIEXPORT void JNICALL Java_CPointer_setLong
  (JNIEnv *env, jobject self, jint index, jlong value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &value, sizeof(value));
}

/*
 * Class:     CPointer
 * Method:    setShort
 * Signature: (IS)V
 */
JNIEXPORT void JNICALL Java_CPointer_setShort
  (JNIEnv *env, jobject self, jint index, jshort value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    memcpy(peer + index, &value, sizeof(value));
}

/*
 * Class:     CPointer
 * Method:    setString
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_CPointer_setString
  (JNIEnv *env, jobject self, jint index, jstring value)
{
    jbyte *peer = (jbyte *)env->GetLongField(self, FID_CPointer_peer);
    char *str = JNU_GetStringNativeChars(env, value);
    if (str == NULL) return;
    strcpy((char *)peer + index, str);
    free(str);
}


/********************************************************************/
/*		     Native methods of class CMalloc		    */
/********************************************************************/

/*
 * Class:     CMalloc
 * Method:    malloc
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_CMalloc_malloc
  (JNIEnv *env, jclass cls, jint size)
{
    return (jlong)malloc(size);
}

/*
 * Class:     CMalloc
 * Method:    free
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CMalloc_free
  (JNIEnv *env, jobject self)
{
    long peer = env->GetLongField(self, FID_CPointer_peer);
    free((void *)peer);
}


/********************************************************************/
/*			   Utility functions			    */
/********************************************************************/

/* Throw an exception by name */
static void 
JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg)
{
    jclass cls = env->FindClass(name);

    if (cls != 0) /* Otherwise an exception has already been thrown */
        env->ThrowNew(cls, msg);

    /* It's a good practice to clean up the local references. */
    env->DeleteLocalRef(cls);
}

/* Translates a Java string to a C string using the String.getBytes 
 * method, which uses default local encoding.
 */
static char *
JNU_GetStringNativeChars(JNIEnv *env, jstring jstr)
{
    jbyteArray hab = 0;
    jthrowable exc;
    char *result = 0;

    hab = (jbyteArray)env->CallObjectMethod(jstr, MID_String_getBytes);
    exc = env->ExceptionOccurred();
    if (!exc) {
        jint len = env->GetArrayLength(hab);
        result = (char *)malloc(len + 1);
	if (result == 0) {
	    JNU_ThrowByName(env, "java/lang/OutOfMemoryError", 0);
	    env->DeleteLocalRef(hab);
	    return 0;
	}
	env->GetByteArrayRegion(hab, 0, len, (jbyte *)result);
	result[len] = 0; /* NULL-terminate */
    } else {
        env->DeleteLocalRef(exc);
    }
    env->DeleteLocalRef(hab);
    return result;
}

/* Constructs a Java string from a C array using the String(byte [])
 * constructor, which uses default local encoding.
 */
static jstring
JNU_NewStringNative(JNIEnv *env, const char *str)
{
    jstring result;
    jbyteArray hab = 0;
    int len;

    len = strlen(str);
    hab = env->NewByteArray(len);
    if (hab != 0) {
        env->SetByteArrayRegion(hab, 0, len, (jbyte *)str);
	result = (jstring)env->NewObject(Class_String,
				   MID_String_init, hab);
	env->DeleteLocalRef(hab);
	return result;
    }
    return 0;
}

/* Canonicalize NULL pointers */
static jobject makeCPointer(JNIEnv *env, void *p)
{
    jobject obj;
    if (p == NULL) {
        return NULL;
    }
    obj = env->AllocObject(Class_CPointer);
    if (obj) {
        env->SetLongField(obj, FID_CPointer_peer, (jlong)p);
    }
    return obj;
}
