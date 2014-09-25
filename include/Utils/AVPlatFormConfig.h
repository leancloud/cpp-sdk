/**
 * @file   AVPlatFormConfig.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:52:24 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */


#ifndef INCLUDE_UTILS_AVPLATFORMCONFIG_H_
#define INCLUDE_UTILS_AVPLATFORMCONFIG_H_

// define supported target platform macro which AVOSCloud uses.

#define AV_PLATFORM_UNKNOWN            0
#define AV_PLATFORM_IOS                1
#define AV_PLATFORM_ANDROID            2
#define AV_PLATFORM_WIN32              3
#define AV_PLATFORM_MARMALADE          4
#define AV_PLATFORM_LINUX              5
#define AV_PLATFORM_BADA               6
#define AV_PLATFORM_BLACKBERRY         7
#define AV_PLATFORM_MAC                8
#define AV_PLATFORM_NACL               9
#define AV_PLATFORM_EMSCRIPTEN        10
#define AV_PLATFORM_TIZEN             11
#define AV_PLATFORM_QT5               12
#define AV_PLATFORM_WP8               13
#define AV_PLATFORM_WINRT             14

// Determine target platform by compile environment macro.
#define AV_TARGET_PLATFORM             AV_PLATFORM_UNKNOWN


// mac
#if defined(AV_TARGET_OS_MAC)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_MAC
#endif

// iphone
#if defined(AV_TARGET_OS_IPHONE)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_IOS
#endif

// android
#if defined(ANDROID)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_ANDROID
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM         AV_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_TIZEN
#endif

// qt5
#if defined(AV_TARGET_QT5)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_QT5
#endif

// WinRT (Windows Store App)
#if defined(WINRT)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_WINRT
#endif

// WP8 (Windows Phone 8 App)
#if defined(WP8)
#undef  AV_TARGET_PLATFORM
#define AV_TARGET_PLATFORM     AV_PLATFORM_WP8
#endif

#endif  // INCLUDE_UTILS_AVPLATFORMCONFIG_H_
