#ifndef WRAP__CONFIG__H__
#define WRAP__CONFIG__H__

//#define NETUTIL_IOS_PHONE 
///////////////////////////////////////////////////NETUTIL_ANDROID_PHONE
#ifdef NETUTIL_IOS_PHONE

//使用MAC xcode编译,如需指定手机需定义下面的宏
#define NETUTIL_MAC
//IOS版本定义
#define NETUTIL_IOS

#else

//使用Linux环境
#define NETUTIL_LINUX
//使用NDK编译
#define NETUTIL_ANDROID

#endif




#ifdef NETUTIL_ANDROID
#include <jniloghelp.h>
#endif

#endif//WRAP__CONFIG__H__
