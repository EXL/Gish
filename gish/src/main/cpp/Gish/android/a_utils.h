#ifndef A_UTILS_H
#define A_UTILS_H

#include "../config.h"

extern JNIEnv *javaEnviron;
extern void initializeJavaEnviron();
extern void doVibrateFromJNI(int duration);
extern const char* getAssetsPathFromJNI();
extern void updateGameConfigFromJNI();
extern void openUrlFromJNI(const char *url);

#endif // A_UTILS_H
