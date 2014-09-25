/**
 * @file   UploaderManager.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Mon Aug 18 14:49:00 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */
#include "AVFile/UploaderManager.h"

#include <cstdlib>
#include <string>
#include <exception>
#include <sstream>
#include <boost/log/trivial.hpp>

#include "json/json.h"
#include "Request/AVPaasClient.h"

NS_AV_BEGIN

UploaderManager* UploaderManager::_instance = nullptr;
std::mutex UploaderManager::_lock;

UploaderManager* UploaderManager::sharedInstance() {
  std::lock_guard<std::mutex> locker(_lock);
  if (_instance == nullptr) {
    _instance = new UploaderManager();
  }

  return _instance;
}

std::string UploaderManager::generateRandomString(int length) {
  std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::string randomString;

  for (int i = 0; i < length; ++i) {
    randomString.append(StringUtils::string_format("%C", letters[rand() % letters.length()]));
  }

  return randomString;
}

std::string UploaderManager::generateQiniuKey() {
  return UploaderManager::generateRandomString(16);
}

void UploaderManager::uploadWithAVFileAndCallback(AVFile* file,
                                                  AVBooleanResultCallback resultCallback) {
  std::string key = UploaderManager::generateQiniuKey();
  std::string extension = StringUtils::getExtentionFromFileName(file->name);
  if (extension.length() > 0) {
    key.append(StringUtils::string_format(".%s", extension));
  }

  Json::Value dict;
  dict["key"] = key;
  dict["name"] = file->name;
  dict["mime_type"] = "application/octet-stream";
  dict["__type"] = "File";
  dict["metaData"] = file->updateMetadata();

  AVPaasClient::sharedInstance()->
      postObject("qiniu", dict, [&](Json::Value const & root, AVError const & error) {
          if (error.domain.length() == 0) {
            std::string token = root["token"].asString();
            std::string bucket = root["bucket"].asString();
            std::string objectId = root["objectId"].asString();

            file->url = root["url"].asString();
            file->bucket = bucket;
            file->objectId = objectId;

            this->uploadFileToBucketWithTokenAndCallback(bucket, token, file, key, [&](bool const& succeeded, AVError const& error) {
                if (error.domain.length() == 0) {
                  if (file->name.length() <= 0) {
                    file->name = objectId;
                  }
                  BOOST_LOG_TRIVIAL(info) << "remote file link:" << file->url;
                }

                resultCallback(succeeded, error);
              });
          } else {
            resultCallback(false, error);
          }
        });
}

void UploaderManager::uploadFileToBucketWithTokenAndCallback(std::string bucket,
                                                             std::string token,
                                                             AVFile* file,
                                                             std::string key,
                                                             AVBooleanResultCallback resultCallback) {
  std::string bodyStr = this->generateQiniuMultipartBodyString(bucket, token, file, key);
  AVPaasClient::sharedInstance()->uploadFileToQiniuWithBodyAndCallback(bodyStr, resultCallback);
}

void UploaderManager::release() {
  AV_SAFE_DELETE(this);
}

////////////////////////////// private methods  //////////////////////////////
std::string UploaderManager::generateQiniuMultipartBodyString(std::string bucket,
                                               std::string token,
                                               AVFile* file,
                                               std::string key) {
  try {
    std::string k_crlf = "\r\n";
    std::string startBoundaryStr = "--28e84231563f43b08b1cc55659e9b3ac";
    std::string endBoundaryStr = "--28e84231563f43b08b1cc55659e9b3ac--";

    std::ostringstream os;

    os << startBoundaryStr << k_crlf;
    os << "Content-Disposition: form-data; name=\"token\"" << k_crlf << k_crlf;
    os << token << k_crlf;

    os << startBoundaryStr << k_crlf;
    os << "Content-Disposition: form-data; name=\"key\"" << k_crlf << k_crlf;
    os << key << k_crlf;

    std::ifstream ifs(file->path);
    ifs >> std::noskipws;
    std::string fileStr((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    os << startBoundaryStr << k_crlf;
    os << StringUtils::string_format("Content-Disposition: %s",
                                     StringUtils::string_format("form-data; name=\"file\"; filename=\"%s\"",
                                                                file->name))
       << k_crlf << k_crlf;

    os << fileStr << k_crlf;

    os << endBoundaryStr;

    return os.str();
  } catch (std::exception & e) {
    BOOST_LOG_TRIVIAL(error) << e.what();

    std::string str;
    return str;
  }
}

NS_AV_END
