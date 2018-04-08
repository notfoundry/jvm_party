/*
* Created by Mark Johnson on 7/3/2017.
*/

#ifndef PARTY_JAVAENVIRONMENT_H
#define PARTY_JAVAENVIRONMENT_H

#include <jni.h>
#include <jvmti.h>

using JVM_Lookup_Function = jint (JNICALL*)(JavaVM**, jsize, jsize*);

struct JavaEnv {
    JNIEnv* jni_env;
    jvmtiEnv* jvmti_env;
};

jint makeJavaEnvironment(JVM_Lookup_Function jvmLookup, JavaEnv& envDestination);

#endif //PARTY_JAVAENVIRONMENT_H
