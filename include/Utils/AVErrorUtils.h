/**
 * @file   AVErrorUtils.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Fri Jul 25 16:02:20 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_UTILS_AVERRORUTILS_H_
#define INCLUDE_UTILS_AVERRORUTILS_H_

#include <string>
#include <unordered_map>
#include "json/json.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

extern std::string const kAVErrorDomain;
extern std::string const kAVErrorUnknownText;

struct AVError {
  std::string domain;
  int code;
  std::unordered_map<std::string, std::string> userInfo;

 AVError()
 :domain(), code(), userInfo()
  { }

 AVError(std::string const & _domain, int const & _code)
 :domain(_domain), code(_code)
  { }

 AVError(std::string const & _domain, int const & _code, std::unordered_map<std::string, std::string> const & _userInfo)
 :domain(_domain), code(_code), userInfo(_userInfo)
  { }
};

class AVErrorUtils {
public:
  static AVError errorWithCode(int const & code);
  static AVError errorWithCode(int const & code, std::string const & errorText);
  static AVError internalServerError();
  static AVError errorFromJSON(Json::Value json);
  static std::string errorTextFromError(AVError & error);

private:
  static bool _isDictionaryError(Json::Value const & json);
  static AVError _errorFromDictionary(Json::Value const & json);
};


NS_AV_END

#endif  // INCLUDE_UTILS_AVERRORUTILS_H_
