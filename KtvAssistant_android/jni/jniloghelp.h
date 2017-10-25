/*
 *
 * 		2014-4-16
 * 		by glp
 * 		This file is used for debugging in eclipse to print some thing in the LogCat
 * 		Just like printf in th c.
 *
 */

#ifndef JNILOGHELP__H__
#define JNILOGHELP__H__
#ifdef _WIN32
// VERBOSE
#define LOGV(...) (void(0))
// DEBUG
#define LOGD(...) (void(0))
// INFO
#define LOGI(...) (void(0))
//WARN
#define LOGW(...) (void(0))
// ERROR
#define LOGE(...) (void(0))
#else
#include <android/log.h>
// VERBOSE
#define LOGV(...)   __android_log_print(ANDROID_LOG_VERBOSE, " JniNetUtil", __VA_ARGS__)
// DEBUG
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG , " JniNetUtil ", __VA_ARGS__)
// INFO
#define LOGI(...)    __android_log_print(ANDROID_LOG_INFO  , " JniNetUtil ",__VA_ARGS__)
//WARN
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN  , " JniNetUtil ", __VA_ARGS__)
// ERROR
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR  , " JniNetUtil ",__VA_ARGS__)

#define LOGGLP(...)  __android_log_print(ANDROID_LOG_INFO  , " GLP ",__VA_ARGS__)
#endif

#endif//JNILOGHELP__H__
