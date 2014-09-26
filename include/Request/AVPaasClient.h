/**
 * @file   AVPaasClient.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:51:14 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_REQUEST_AVPAASCLIENT_H_
#define INCLUDE_REQUEST_AVPAASCLIENT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <boost/network/protocol/http/client.hpp>
#include "Utils/AVPlatformMacros.h"
#include "Utils/AVStringUtils.h"
#include "AVConstants.h"
#include "AVObject/AVObject.h"
#include "User/AVUser.h"
#include "ACL/AVACL.h"

NS_AV_BEGIN

namespace network = boost::network;
namespace http = boost::network::http;
namespace uri = boost::network::uri;

class AVPaasClient {
public:
  typedef Json::Value map;
  typedef std::unordered_map<std::string, std::string> stringMap;
  http::client clientImpl;
  std::string applicationId;
  std::string clientKey;
  std::string baseURL;
  std::string apiVersion;
  std::string apiKeyField;
  std::string applicationIdField;
  std::string sessionTokenField;
  AVUser* currentUser;
  AVACL* defaultACL;
  bool currentUserAccessForDefaultACL;
  int timeoutInterval;
  bool productionMode;
  bool isLastModify;

private:
  static AVPaasClient *_instance;
  static std::recursive_mutex _lock;
  stringMap _headerMap;
  bool _requestProcessed;
  std::string _responseBody;

private:
  AVPaasClient();
  void updateHeaders();
  void addHeaderMap(stringMap const && headerMap);
  void processResponse(http::client::response const & response,
                       AVIdResultCallback callback);

public:
  ~AVPaasClient();
  static AVPaasClient* sharedInstance();
  void release();
  void clearLastModifyCache();
  bool isUrulu();
  void useAVOSCloudUS();
  void useAVOSCloudCN();
  void useServer(std::string const & baseURL);

  static map batchMethod(std::string const & method,
                         std::string const & path,
                         map const & body,
                         map const & parameters);

  static void updateBatchMethod(std::string const & method,
                                std::string const & path,
                                map & map);

  void getObject(std::string const & path,
                 map const & parameters,
                 AVIdResultCallback callback);

  void getObject(std::string const & path,
                 map const & parameters,
                 AVCachePolicy policy,
                 int maxCacheAge,
                 AVIdResultCallback callback);

  void putObject(std::string const & path,
                 map const & parameters,
                 std::string const & sessionToken,
                 AVIdResultCallback callback);

  void postBatchObject(std::vector<map> const & parameterArray,
                       AVVectorResultCallback callback);

  void postBatchObject(std::vector<map> const & parameterArray,
                       map const & headerMap,
                       bool const & isEventually,
                       AVVectorResultCallback callback);

  void postBatchSaveObject(std::vector<map> const & parameterArray,
                           map const & headerMap,
                           bool const & isEventually,
                           AVIdResultCallback callback);

  void postObject(std::string const & path,
                  map const & parameters,
                  AVIdResultCallback callback);

  void postObject(std::string const & path,
                  map const & parameters,
                  bool const & isEventually,
                  AVIdResultCallback callback);

  void deleteObject(std::string const & path,
                    map const & parameters,
                    AVIdResultCallback callback);

  void deleteObject(std::string const & path,
                    map const & parameters,
                    bool const & isEventually,
                    AVIdResultCallback callback);

  void deleteObjects(std::vector<AVObject> const & objects,
                     bool const & isEventually,
                     AVIdResultCallback callback);

  void cancelQuery(std::string const & path, map const & parameters);

  std::string absoluteStringFromPath(std::string const & path,
                                     map const & parameters);

  void handleAllArchiveRequest();

  http::client::request createRequest(std::string const & path,
                                      map const & parameters,
                                      bool isQuery);

  void uploadFileToQiniuWithBodyAndCallback(std::string const & body,
                                            AVBooleanResultCallback callback);

  void fetchFileDataIntoPathWithUrl(std::string const & path,
                                    std::string const & url);
};

NS_AV_END

#endif  // INCLUDE_REQUEST_AVPAASCLIENT_H_
