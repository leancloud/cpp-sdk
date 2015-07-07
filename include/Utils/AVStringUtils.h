/**
 * @file   AVStringUtils.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Jul 10 18:24:02 2014
 *
 * @brief  common helper functions for string
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_UTILS_AVSTRINGUTILS_H_
#define INCLUDE_UTILS_AVSTRINGUTILS_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <boost/format.hpp>

#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define SHA1 CC_SHA1
#else
#include <openssl/md5.h>
#endif

#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

namespace StringUtils {

  template<class TFirst>
      inline void string_format(boost::format & fmt, TFirst && first) {
    fmt % first;
  }

  template<class TFirst, class... TOther>
      inline void string_format(boost::format & fmt, TFirst && first, TOther&&... other) {
    fmt % first;
    string_format(fmt, other...);
  }

  template<class TFirst, class... TOther>
      inline std::string string_format(const char* format, TFirst&& first, TOther&&... other) {
    boost::format fmt(format);
    string_format(fmt, first, other...);
    return fmt.str();
  }

  inline std::string AVMD5String(std::string const &rawStr) {
    const char *str = rawStr.c_str();
    int length = strlen(str);


    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
      if (length > 512) {
        MD5_Update(&c, str, 512);
      } else {
        MD5_Update(&c, str, length);
      }
      length -= 512;
      str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
      snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    std::string result(out);
    free(out);

    return result;
  }

  inline std::string getFileNameFromPath(std::string const & path) {
    std::size_t found = path.find_last_of("/\\");

    if (found != std::string::npos) {
      return path.substr(found + 1);
    }

    std::string fileName("");
    return fileName;
  }

  inline std::string getExtentionFromFileName(std::string const & name) {
    std::size_t found = name.find_last_of(".");
    if (found != std::string::npos) {
      return name.substr(found + 1);
    }

    std::string extension("");
    return extension;
  }
}  // namespace StringUtils

NS_AV_END

#endif  // INCLUDE_UTILS_AVSTRINGUTILS_H_
