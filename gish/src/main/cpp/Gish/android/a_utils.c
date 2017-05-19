#include "../android/a_utils.h"
#include "../game/config.h"
#include "../game/game.h"

JNIEnv *javaEnviron = NULL;

void initializeJavaEnviron()
{
    javaEnviron = SDL_AndroidGetJNIEnv();
}

void doVibrateFromJNI(int duration)
{
    if (javaEnviron != NULL) {
        jclass clazz = (*javaEnviron)->FindClass(javaEnviron, "ru/exlmoto/aaaa/GishActivity");
        if (clazz == 0) {
            TO_DEBUG_LOG("Error JNI: Class ru/exlmoto/aaaa/GishActivity not found!");
            return;
        }

        jmethodID methodId = (*javaEnviron)->GetStaticMethodID(javaEnviron, clazz, "doVibrate", "(II)V");
        if (methodId == 0) {
            TO_DEBUG_LOG("Error JNI: methodId is 0, method doVibrate (II)V not found!");
            return;
        }

        // Call Java-method
        (*javaEnviron)->CallStaticVoidMethod(javaEnviron, clazz, methodId, (jint)duration, 1);

        // Delete Ref
        (*javaEnviron)->DeleteLocalRef(javaEnviron, clazz);
    }
}

void updateGameConfigFromJNI()
{
    if (javaEnviron != NULL) {
        jclass clazz = (*javaEnviron)->FindClass(javaEnviron, "ru/exlmoto/gish/GishLauncherActivity$GishSettings");
        if (clazz == 0) {
            TO_DEBUG_LOG("Error JNI: Class ru/exlmoto/gish/GishLauncherActivity$GishSettings not found!");
            return;
        }

        // Get sound Field ID
        jfieldID fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "sound", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean sound not found!");
            return;
        }
        jboolean sound = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        config.sound = (int)sound;

        // Get music Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "music", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean music not found!");
            return;
        }
        jboolean music = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        config.music = (int)music;

        // Get showFps Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "showFps", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean showFps not found!");
            return;
        }
        jboolean showFps = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        fps_enabled = (int)showFps;

        // Get fixCache Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "fixCache", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean fixCache not found!");
            return;
        }
        jboolean fixCache = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        cache_fix = (int)fixCache;

        // Get joyAccel Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "joyAccel", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean joyAccel not found!");
            return;
        }
        jboolean joyAccel = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        config.joystick = (int)joyAccel;

        // Get lights Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "lights", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean lights not found!");
            return;
        }
        jboolean lights = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        lighting_enabled = (int)lights;

        // Get shadows Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "shadows", "Z");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field boolean shadows not found!");
            return;
        }
        jboolean shadows = (*javaEnviron)->GetStaticBooleanField(javaEnviron, clazz, fieldID);
        shadow_enabled = (int)shadows;

        // Get touchControls Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "touchControls", "I");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field int touchControls not found!");
            return;
        }
        jint touch_Controls = (*javaEnviron)->GetStaticIntField(javaEnviron, clazz, fieldID);
        touchcontrols = (int)touch_Controls;

        // Get zoom Field ID
        fieldID = (*javaEnviron)->GetStaticFieldID(javaEnviron, clazz, "zoom", "F");
        if (fieldID == 0) {
            TO_DEBUG_LOG("Error JNI: fieldID is 0, field float zoom not found!");
            return;
        }
        jfloat zoom = (*javaEnviron)->GetStaticFloatField(javaEnviron, clazz, fieldID);
        z_zoom = (float)zoom;

        // Delete Ref
        (*javaEnviron)->DeleteLocalRef(javaEnviron, clazz);
    }
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
