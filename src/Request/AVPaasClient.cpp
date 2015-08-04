/**
 * @file   AVPaasClient.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:51:29 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "Request/AVPaasClient.h"

#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <thread>
#include <exception>
#include <boost/log/trivial.hpp>

#define kAVDefaultNetworkTimeoutInterval 10.0

NS_AV_BEGIN

const std::string API_VERSION = "1.1";
const std::string QiniuServerPath = "http://up.qiniu.com";

AVPaasClient* AVPaasClient::_instance = nullptr;
std::recursive_mutex AVPaasClient::_lock;

AVPaasClient::AVPaasClient()
    :clientImpl(),
     baseURL("https://api.leancloud.cn"),
     apiVersion(API_VERSION),
     apiKeyField("X-avoscloud-Application-Key"),
     applicationIdField("X-avoscloud-Application-Id"),
     timeoutInterval(kAVDefaultNetworkTimeoutInterval),
     sessionTokenField("X-avoscloud-Session-Token"),
     productionMode(true),
     currentUser(nullptr),
     defaultACL(nullptr),
     _headerMap(),
     _responseBody() {
}

AVPaasClient::~AVPaasClient() {
}

AVPaasClient* AVPaasClient::sharedInstance() {
  std::lock_guard<std::recursive_mutex> locker(_lock);
  if (_instance == nullptr) {
    _instance = new AVPaasClient();
  }

  return _instance;
}

void AVPaasClient::release() {
  AV_SAFE_DELETE(this);
}

void AVPaasClient::clearLastModifyCache() {
}

bool AVPaasClient::isUrulu() {
  return (this->applicationIdField).compare("X-avoscloud-Application-Id") == 0;
}

void AVPaasClient::useAVOSCloudUS() {
  this->baseURL = "https://us-api.leancloud.cn";
}

void AVPaasClient::useAVOSCloudCN() {
  this->baseURL = "https://api.leancloud.cn";
}

void AVPaasClient::useServer(std::string const & baseURL) {
  this->baseURL = baseURL;
  this->applicationIdField = "X-avoscloud-Application-Id";
  this->apiKeyField = "X-avoscloud-Application-Key";
  this->sessionTokenField = "X-avoscloud-Session-Token";
}

void AVPaasClient::addHeaderMap(stringMap const && headerMap) {
  for (auto it = headerMap.begin(); it != headerMap.end(); ++it) {
    this->_headerMap[it->first] = it->second;
  }
}

void AVPaasClient::updateHeaders() {
  time_t now;
  time(&now);
  std::string timestamp = StringUtils::string_format("%ld", now);
  std::string signCandidate = StringUtils::string_format("%s%s", timestamp, this->clientKey);
  std::string sign = StringUtils::AVMD5String(signCandidate);
  std::string headerValue = StringUtils::string_format("%s,%s", sign, timestamp);

  std::unordered_map<std::string, std::string> _map;
  _map["x-avoscloud-request-sign"] = headerValue;
  _map[this->applicationIdField] = this->applicationId;
  _map["Accept"] = "application/json";
  _map["User-Agent"] = "AVOS Cloud C++ SDK";
  _map["X-avoscloud-Application-Production"] = this->productionMode ? "1" : "0";
  _map["Content-Type"] = "application/json";
  if (this->currentUser != nullptr && this->currentUser->sessionToken.length() > 0) {
    _map[this->sessionTokenField] = this->currentUser->sessionToken;
  }

  this->addHeaderMap(std::move(_map));
}

AVPaasClient::map AVPaasClient::batchMethod(std::string const & method,
                                            std::string const & path,
                                            AVPaasClient::map const & body,
                                            AVPaasClient::map const & parameters) {
  std::string myPath = StringUtils::string_format("/%s/%s",
                                                  AVPaasClient::sharedInstance()->apiVersion,
                                                  path);

  AVPaasClient::map result;
  result["method"] = method;
  result["path"] = path;

  return result;
}

void AVPaasClient::updateBatchMethod(std::string const & method,
                                     std::string const & path,
                                     map & map) {
  std::string myPath = StringUtils::string_format("/%s/%s",
                                                  AVPaasClient::sharedInstance()->apiVersion,
                                                  path);
  map["method"] = method;
  map["path"] = myPath;
}

http::client::request AVPaasClient::createRequest(std::string const & path,
                                                  map const & parameters,
                                                  bool isQuery) {
  uri::uri base_uri(StringUtils::string_format("%s/%s/%s",
                                               this->baseURL,
                                               this->apiVersion,
                                               path));
  uri::uri request_uri;
  request_uri << base_uri;

  if (isQuery) {
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
      std::string key = it.key().asString();
      std::string value = (*it).asString();
      request_uri << uri::query(uri::encoded(key), uri::encoded(value));
    }
  }

  http::client::request request(request_uri);

  for (auto it = _headerMap.begin(); it != _headerMap.end(); ++it) {
    std::string key = it->first;
    std::string value = it->second;
    request << network::header(key, value);
  }

  return request;
}

void AVPaasClient::processResponse(http::client::response const & response,
                                   AVIdResultCallback callback) {
  Json::Value result;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(response.body().c_str(), result);

  if (parsingSuccessful) {
    AVError error = AVErrorUtils::errorFromJSON(result);
    callback(result, error);
  }
}

void AVPaasClient::getObject(std::string const & path,
                             map const & parameters,
                             AVIdResultCallback callback) {
  std::lock_guard<std::recursive_mutex> locker(_lock);

  try {
    this->updateHeaders();
    http::client::request request = this->createRequest(path, parameters, true);
    http::client::response response = this->clientImpl.get(request);

    processResponse(response, callback);
  } catch (std::exception &e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void AVPaasClient::putObject(std::string const & path,
                             map const & parameters,
                             std::string const & sessionToken,
                             AVIdResultCallback callback) {
  std::lock_guard<std::recursive_mutex> locker(_lock);

  try {
    this->updateHeaders();

    AVPaasClient::map myParameters;
    http::client::request request = this->createRequest(path, myParameters, false);

    http::client::response response = this->clientImpl.put(request, parameters.toStyledString());

    processResponse(response, callback);
  } catch (std::exception &e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void AVPaasClient::postObject(std::string const & path,
                              map const & parameters,
                              AVIdResultCallback callback) {
  std::lock_guard<std::recursive_mutex> locker(_lock);

  try {
    this->updateHeaders();
    AVPaasClient::map myParameters;
    http::client::request request = this->createRequest(path, myParameters, false);

    http::client::response response = this->clientImpl.post(request, parameters.toStyledString());

    processResponse(response, callback);
  } catch (std::exception &e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void AVPaasClient::deleteObject(std::string const & path,
                                map const & parameters,
                                AVIdResultCallback callback) {
  std::lock_guard<std::recursive_mutex> locker(_lock);

  try {
    this->updateHeaders();
    http::client::request request = this->createRequest(path, parameters, false);
    http::client::response response = this->clientImpl.delete_(request);

    processResponse(response, callback);
  } catch (std::exception &e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void AVPaasClient::uploadFileToQiniuWithBodyAndCallback(std::string const & body,
                                                        AVBooleanResultCallback callback) {
  try {
    http::client client;
    uri::uri request_uri(QiniuServerPath);

    http::client::request request(request_uri);
    request << network::header("Content-Type",
                               "multipart/form-data; boundary=28e84231563f43b08b1cc55659e9b3ac");

    http::client::response response = client.post(request, body);

    std::cout << response.body() << std::endl;

    if (response.status() == 200) {
      AVError error;
      callback(true, error);
    } else {
      AVError error(kAVErrorDomain, kAVErrorUnsavedFile);
      callback(false, error);
    }
  } catch (std::exception & e) {
    AVError error(kAVErrorDomain, kAVErrorUnsavedFile);
    callback(false, error);
  }
}

void AVPaasClient::fetchFileDataIntoPathWithUrl(std::string const & path,
                                                std::string const & url) {
  if (url.length() > 0) {
    try {
      http::client client;
      uri::uri request_uri(url);

      http::client::request request(request_uri);
      http::client::response response = client.get(request);

      std::ofstream ofs(path);
      ofs << response.body();
      ofs.close();
    } catch (std::exception & e) {
      BOOST_LOG_TRIVIAL(error) << e.what();
    }
  }
}

NS_AV_END
