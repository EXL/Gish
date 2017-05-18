#include "../android/a_utils.h"

JNIEnv *javaEnviron = NULL;

void initializeJavaEnviron()
{
    javaEnviron = SDL_AndroidGetJNIEnv();
}

const char* getAssetsPathFromJNI()
{
    if (javaEnviron != NULL) {
        jclass clazz = (*javaEnviron)->FindClass(javaEnviron, "ru/exlmoto/gish/GishLauncherActivity$GishSettings");
        if (clazz == 0) {
            TO_DEBUG_LOG("Error JNI: Class ru/exlmoto/gish/GishLauncherActivity$GishSettings not found!");
            return NULL;
        }

        // Get Field ID
        jfieldID fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "gishDataSavedPath", "Ljava/lang/String;");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field String gishDataSavedPath not found!");
            return NULL;
        }

        // Get String from Java and convert to char*
        jstring javaString = (*javaEnviron)->GetStaticObjectField(javaEnviron, clazz, fieldID);
        if (javaString == 0) {
            return NULL;
        }
        const char *nativeString = (*javaEnviron)->GetStringUTFChars(javaEnviron, javaString, 0);
        char *stringToGishEngine = strdup(nativeString);

        // Destroy string
        (*javaEnviron)->ReleaseStringUTFChars(javaEnviron, javaString, nativeString);

        // Delete Ref
        (*javaEnviron)->DeleteLocalRef(javaEnviron, clazz);

        // Return copy of string to Engine
        return stringToGishEngine;
    } else {
        return "/storage/sdcard0/Gish/";
    }
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
