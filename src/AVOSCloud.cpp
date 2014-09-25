/**
 * @file   AVOSCloud.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:48:54 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include <string>

#include "AVOSCloud.h"
#include "Request/AVPaasClient.h"
#include "AVFile/UploaderManager.h"

NS_AV_BEGIN

void AVOSCloud::setApplicationId(std::string applicationId,
                                 std::string clientKey) {
  AVPaasClient::sharedInstance()->applicationId = applicationId;
  AVPaasClient::sharedInstance()->clientKey = clientKey;
}

std::string AVOSCloud::getApplicationId() {
  return AVPaasClient::sharedInstance()->applicationId;
}

std::string AVOSCloud::getClientKey() {
  return AVPaasClient::sharedInstance()->clientKey;
}

NS_AV_END
