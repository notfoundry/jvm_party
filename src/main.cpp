#include "java_environment.h"

#include <minwindef.h>
#include <libloaderapi.h>

static void JNICALL debugClassFileLoadHook(jvmtiEnv *jvmti_env,
                  JNIEnv* jni_env,
                  jclass class_being_redefined,
                  jobject loader,
                  const char* name,
                  jobject protection_domain,
                  jint class_data_len,
                  const unsigned char* class_data,
                  jint* new_class_data_len,
                  unsigned char** new_class_data) {

    auto system = jni_env->FindClass("java/lang/System");
    auto printstream = jni_env->FindClass("java/io/PrintStream");
    auto out = jni_env->GetStaticFieldID(system, "out", "Ljava/io/PrintStream;");
    auto println = jni_env->GetMethodID(printstream, "println", "(Ljava/lang/String;)V");
    auto obj = jni_env->GetStaticObjectField(system, out);
    jni_env->CallVoidMethod(obj, println, jni_env->NewStringUTF(name));
}

static bool getJVMLookupFunction(JVM_Lookup_Function& func_destination) {
    FARPROC processExport = GetProcAddress(GetModuleHandleA("jvm.dll"), "JNI_GetCreatedJavaVMs");
    if (!processExport) {
        return false;
    }
    func_destination = reinterpret_cast<JVM_Lookup_Function>(processExport);
    return true;
}

static jint configureJavaEnvironment(JavaEnv& env) {
    jint err;

    jvmtiCapabilities capabilities {};
    capabilities.can_retransform_classes = 1;
    capabilities.can_retransform_any_class = 1;

    err = env.jvmti_env->AddCapabilities(&capabilities);

    if (err != JVMTI_ERROR_NONE) {
        return err;
    }

    jvmtiEventCallbacks callbacks{};

    callbacks.ClassFileLoadHook = debugClassFileLoadHook;

    err = env.jvmti_env->SetEventCallbacks(&callbacks, sizeof(callbacks));

    if (err != JVMTI_ERROR_NONE) {
        return err;
    }

    err = env.jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr);

    if (err != JVMTI_ERROR_NONE) {
        return err;
    }

    return JVMTI_ERROR_NONE;
}

static jint initializeJavaEnvironment(JVM_Lookup_Function jvmLookup, JavaEnv& envDestination) {
    JavaEnv env;
    jint envCreationResult = makeJavaEnvironment(jvmLookup, env);
    if (envCreationResult != JNI_OK) {
        return envCreationResult;
    }

    jint envConfigurationResult = configureJavaEnvironment(env);

    if (envConfigurationResult != JNI_OK) {
        return envConfigurationResult;
    }

    envDestination = env;

    return JNI_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            JVM_Lookup_Function jvmLookup;
            bool jvmLookupResult = getJVMLookupFunction(jvmLookup);
            if (!jvmLookupResult) {
                return FALSE;
            }

            JavaEnv env;
            jint err = initializeJavaEnvironment(jvmLookup, env);
            if (err != JVMTI_ERROR_NONE) {
                return FALSE;
            }

            auto system = env.jni_env->FindClass("java/lang/System");
            auto printstream = env.jni_env->FindClass("java/io/PrintStream");
            auto out = env.jni_env->GetStaticFieldID(system, "out", "Ljava/io/PrintStream;");
            auto println = env.jni_env->GetMethodID(printstream, "println", "(Ljava/lang/String;)V");
            auto obj = env.jni_env->GetStaticObjectField(system, out);

            auto foo = env.jni_env->FindClass("java/util/Calendar");

            auto rt = env.jvmti_env->RetransformClasses(1, &foo);

            env.jni_env->CallVoidMethod(obj, println, env.jni_env->NewStringUTF("party time"));
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        default: break;
    }
    return TRUE;
}