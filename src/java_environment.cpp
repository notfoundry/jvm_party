/*
* Created by Mark Johnson on 7/3/2017.
*/

#include "java_environment.h"

jint makeJavaEnvironment(JVM_Lookup_Function jvmLookup, JavaEnv& envDestination) {
    jsize jvmCount = 0;
    jvmLookup(nullptr, 0, &jvmCount);

    JavaVM* jvmBuffer;
    jint javaVMCreateResult = jvmLookup(&jvmBuffer, jvmCount, &jvmCount);
    if (javaVMCreateResult != JNI_OK) {
        return javaVMCreateResult;
    }

    void* jniEnvBuffer;
    jint jniEnvLookupResult = jvmBuffer->AttachCurrentThread(&jniEnvBuffer, nullptr);
    if (jniEnvLookupResult != JNI_OK) {
        return jniEnvLookupResult;
    }

    void* jvmtiEnvBuffer;
    jint jvmtiEnvLookupResult = jvmBuffer->GetEnv(&jvmtiEnvBuffer, JVMTI_VERSION);
    if (jvmtiEnvLookupResult != JNI_OK) {
        return jvmtiEnvLookupResult;
    }

    envDestination = JavaEnv{static_cast<JNIEnv*>(jniEnvBuffer), static_cast<jvmtiEnv*>(jvmtiEnvBuffer)};

    return JNI_OK;
}
