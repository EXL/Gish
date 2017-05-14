#ifndef GISH_CONFIG_H
#define GISH_CONFIG_H

#ifdef __linux__
 #define LINUX 1
 #ifndef ANDROID_NDK
  #define TO_DEBUG_LOG(...) fprintf(stderr, __VA_ARGS__)
 #else
  #include <android/log.h>

  #define LOG_TAG "Gish_jni"
  #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
  #define TO_DEBUG_LOG(...) LOGI(__VA_ARGS__)
 #endif
#endif

#ifdef _WIN32
  #define WINDOWS 1
#endif

#ifdef __APPLE__
  #define MAC 1
#endif

#ifdef MAC
  #define __MACOSX__
#endif

#ifdef WINDOWS
  #define INLINE _inline
#else
  #define INLINE inline
#endif

typedef enum {FALSE = 0, TRUE = 1} bool;

#endif /* GISH_CONFIG_H */
