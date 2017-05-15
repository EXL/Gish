#include "../android/a_utils.h"

JNIEnv *javaEnviron = NULL;

void initializeJavaEnviron()
{
    javaEnviron = SDL_AndroidGetJNIEnv();
}

const char* getAssetsPathFromJNI()
{
    return "/storage/sdcard1/Gish/";
}

void openUrlFromJNI(const char *url)
{
    if (javaEnviron != NULL) {
        jclass clazz = (*javaEnviron)->FindClass(javaEnviron, "ru/exlmoto/gish/GishActivity");
        if (clazz == 0) {
            TO_DEBUG_LOG("Error JNI: Class ru/exlmoto/gish/GishActivity not found!");
            return;
        }

        jmethodID methodId = (*javaEnviron)->GetStaticMethodID(javaEnviron, clazz, "openUrl", "(Ljava/lang/String;)V");
        if (methodId == 0) {
            TO_DEBUG_LOG("Error JNI: methodId is 0, method openUrl (Ljava/lang/String;)V not found!");
            return;
        }

        jstring jurl = (*javaEnviron)->NewStringUTF(javaEnviron, url);

        // Call Java-method
        (*javaEnviron)->CallStaticVoidMethod(javaEnviron, clazz, methodId, jurl);

        // Delete Refs
        (*javaEnviron)->DeleteLocalRef(javaEnviron, jurl);
        (*javaEnviron)->DeleteLocalRef(javaEnviron, clazz);
    }
}
