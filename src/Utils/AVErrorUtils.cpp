/**
 * @file   AVErrorUtils.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Fri Jul 25 16:22:06 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "Utils/AVErrorUtils.h"

#include <string>
#include "AVConstants.h"

NS_AV_BEGIN

std::string const kAVErrorDomain ="AVOS Cloud Error Domain";
std::string const kAVErrorUnknownText = "Error Infomation Unknown";

AVError AVErrorUtils::errorWithCode(int const & code) {
  return AVError(kAVErrorDomain, code);
}

AVError AVErrorUtils::errorWithCode(int const & code,
                                    std::string const & errorText) {
  std::unordered_map<std::string, std::string> userInfo{{"error", errorText}};
  return AVError(kAVErrorDomain, code, userInfo);
}

AVError AVErrorUtils::internalServerError() {
  return AVError(kAVErrorDomain, kAVErrorInternalServer);
}

AVError AVErrorUtils::errorFromJSON(Json::Value json) {
  AVError returnError;
  if (json.type() == Json::nullValue) {
    returnError = AVErrorUtils::internalServerError();
  } else if (json.type() == Json::objectValue) {
    if (AVErrorUtils::_isDictionaryError(json)) {
      returnError = AVErrorUtils::_errorFromDictionary(json);
    } else {
      for (auto it = json.begin(); it != json.end(); ++it) {
        Json::Value child = *it;
        if (child.type() == Json::objectValue && AVErrorUtils::_isDictionaryError(child)) {
          returnError = AVErrorUtils::_errorFromDictionary(child);
          break;
        }
      }
    }
  } else if (json.type() == Json::arrayValue) {
    for (auto it = json.begin(); it != json.end(); ++it) {
      Json::Value child = *it;
      returnError = AVErrorUtils::errorFromJSON(child);
      if (returnError.domain.length() > 0) {
        break;
      }
    }
  }

  return returnError;
}

std::string AVErrorUtils::errorTextFromError(AVError & error) {
  return error.userInfo["error"];
}

AVError AVErrorUtils::_errorFromDictionary(Json::Value const & json) {
  std::string erroString = json["error"].asString();
  int code = json["code"].asInt();

  if (erroString.length() == 0) {
    erroString = kAVErrorUnknownText;
  }

  return AVErrorUtils::errorWithCode(code, erroString);
}

bool AVErrorUtils::_isDictionaryError(Json::Value const & json) {
  if (json.isMember("code")) {
    return true;
  }

  if (json.isMember("error")) {
    return true;
  }

  return false;
}

NS_AV_END
