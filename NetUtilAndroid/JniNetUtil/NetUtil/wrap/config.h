#ifndef WRAP__CONFIG__H__
#define WRAP__CONFIG__H__

//平台版本 ios android
//#define NETUTIL_IOS_PHONE 

//GLP 支持64位
//#define __x86_64__

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

//定义是否是cocos 项目工程
//#define COCOS_PROJECT

#ifdef NETUTIL_ANDROID
#include "jniloghelp.h"
#endif

#endif//WRAP__CONFIG__H__
