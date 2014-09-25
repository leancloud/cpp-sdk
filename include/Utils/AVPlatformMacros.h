/**
 * @file   AVPlatformMacros.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:52:41 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_UTILS_AVPLATFORMMACROS_H_
#define INCLUDE_UTILS_AVPLATFORMMACROS_H_

// generic macros

// namespace avoscloud {}
#ifdef __cplusplus
#define NS_AV_BEGIN                     namespace avoscloud {
#define NS_AV_END                       }
#define USING_NS_AV                     using namespace avoscloud
#else
#define NS_AV_BEGIN
#define NS_AV_END
#define USING_NS_AV
#endif

#define AV_SAFE_DELETE(p)           do { delete (p);} while(0)
#define AV_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); } } while(0)

#endif  // INCLUDE_UTILS_AVPLATFORMMACROS_H_
