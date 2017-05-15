#ifndef A_UTILS_H
#define A_UTILS_H

#include "../config.h"

extern JNIEnv *javaEnviron;
extern void initializeJavaEnviron();
extern void openUrlFromJNI(const char *url);

#endif // A_UTILS_H
