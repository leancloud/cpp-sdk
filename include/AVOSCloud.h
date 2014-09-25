/**
 * @file   AVOSCloud.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:48:16 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */


#ifndef INCLUDE_AVOSCLOUD_H_
#define INCLUDE_AVOSCLOUD_H_

#include <iostream>
#include <string>
#include "AVConstants.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

class AVOSCloud {
public:
  /**
   * Sets the applicationId and clientKey of your application.
   *
   * @param applicationId The applicaiton id for your AVOS Cloud application.
   * @param clientKey The client key for your AVOS Cloud application.
   */
  static void setApplicationId(std::string applicationId,
                               std::string clientKey);

  /**
   *  get Application Id
   *
   *  @return Application Id
   */
  static std::string getApplicationId();

  /**
   *  get Client Key
   *
   *  @return Client Key
   */
  static std::string getClientKey();
};

NS_AV_END

#endif  // INCLUDE_AVOSCLOUD_H_
