/* Note: This program only works on Win32. */
#include <windows.h>
#include <jni.h>
JavaVM *jvm; /* The virtual machine instance */

#define PATH_SEPARATOR ';'
#define USER_CLASSPATH "." /* where Prog.class is */

void thread_fun(void *arg)
{
    jint res;
    jclass cls;
    jmethodID mid;
    jstring jstr;
    jclass stringClass;
    jobjectArray args;
    JNIEnv *env;
    char buf[100];
    int threadNum = (int)arg;
    /* Pass NULL as the third argument */
#ifdef JNI_VERSION_1_2
    res = (*jvm)->AttachCurrentThread(jvm, (void**)&env, NULL);
#else
    res = (*jvm)->AttachCurrentThread(jvm, &env, NULL);
#endif
    if (res < 0) {
       fprintf(stderr, "Attach failed\n");
       return;
    }
    cls = (*env)->FindClass(env, "Prog");
    if (cls == 0) {
        goto detach;
    }
    mid = (*env)->GetStaticMethodID(env, cls, "main", 
                                    "([Ljava/lang/String;)V");
    if (mid == 0) {
        goto detach;
    }
    sprintf(buf, " from Thread %d", threadNum);
    jstr = (*env)->NewStringUTF(env, buf);
    if (jstr == 0) {
        goto detach;
    }
    stringClass = (*env)->FindClass(env, "java/lang/String");
    args = (*env)->NewObjectArray(env, 1, stringClass, jstr);
    if (args == 0) {
        goto detach;
    }
    (*env)->CallStaticVoidMethod(env, cls, mid, args);

 detach:
    if ((*env)->ExceptionOccurred(env)) {
        (*env)->ExceptionDescribe(env);
    }
    (*jvm)->DetachCurrentThread(jvm);
}

int main() {
    JNIEnv *env;
    int i;
    jint res;

#ifdef JNI_VERSION_1_2
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];
    options[0].optionString =
        "-Djava.class.path=" USER_CLASSPATH;
    vm_args.version = 0x00010002;
    vm_args.options = options;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = TRUE;
/* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#else
    JDK1_1InitArgs vm_args;
    char classpath[1024];
    vm_args.version = 0x00010001;
    JNI_GetDefaultJavaVMInitArgs(&vm_args);
    /* Append USER_CLASSPATH to the default system class path */
    sprintf(classpath, "%s%c%s",
            vm_args.classpath, PATH_SEPARATOR, USER_CLASSPATH);
    vm_args.classpath = classpath;
/* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm, &env, &vm_args);
#endif /* JNI_VERSION_1_2 */
if (res < 0) {
        fprintf(stderr, "Can't create Java VM\n");
        exit(1);
    }
    for (i = 0; i < 5; i++)
        /* We pass the thread number to every thread */
        _beginthread(thread_fun, 0, (void *)i);
    Sleep(1000); /* wait for threads to start */
    (*jvm)->DestroyJavaVM(jvm);
    return 0;
}
