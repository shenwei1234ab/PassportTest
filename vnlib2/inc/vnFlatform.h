//
//  vnFlatform.h
//  vnlib
//
//  Created by Wayne on 5/7/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnFlatform_h
#define vnlib_vnFlatform_h

#define VN_PLATFORM_UNKNOWN		0
#define VN_PLATFORM_WIN			1
#define VN_PLATFORM_MAC			2
#define VN_PLATFORM_IOS			3
#define VN_PLATFORM_ANDROID		4
#define VN_PLATFORM_FREEBSD		5
#define VN_PLATFORM_LINUX		6

#define VN_COMPILER_UNKNOWN		0
#define VN_COMPILER_MSVC		1
#define VN_COMPILER_LLVM		2
#define VN_COMPILER_GUNC		3

#if defined(_WIN32)
#	define VN_PLATFORM VN_PLATFORM_WIN
#elif defined(__APPLE__) && defined(__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
#		define VN_PLATFORM VN_PLATFORM_IOS
#	else
#		define VN_PLATFORM VN_PLATFORM_MAC
#	endif
#elif defined(ANDROID)
#	define VN_PLATFORM VN_PLATFORM_ANDROID
#elif defined(__FreeBSD__)
#	define VN_PLATFORM VN_PLATFORM_FREEBSD
#elif defined(__linux__)
#	define VN_PLATFORM VN_PLATFORM_LINUX
#else
#	define VN_PLATFORM VN_PLATFORM_UNKNOWN
#endif

#if defined(_MSC_VER)
#	define VN_COMPILER VN_COMPILER_MSVC
#	define _CRT_SECURE_NO_WARNINGS
#elif defined(__llvm__)
#	define VN_COMPILER VN_COMPILER_LLVM
#elif defined(__GNUC__)
#	define VN_COMPILER VN_COMPILER_GUNC
#else
#	define VN_COMPILER VN_COMPILER_UNKNOWN
#endif

#if defined(_DEBUG) || defined(DEBUG)
#	define VN_DEBUG 1
#endif


#endif
