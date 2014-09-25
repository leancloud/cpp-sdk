/**
 * @file   UploaderManager.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Mon Aug 18 14:34:48 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */
#ifndef INCLUDE_AVFILE_UPLOADERMANAGER_H_
#define INCLUDE_AVFILE_UPLOADERMANAGER_H_

#include <string>
#include <mutex>
#include <condition_variable>
#include "AVFile.h"
#include "AVConstants.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

class UploaderManager {
private:
  static UploaderManager* _instance;
  static std::mutex _lock;

private:
  UploaderManager(){}
  void uploadFileToBucketWithTokenAndCallback(std::string bucket,
                                              std::string token,
                                              AVFile* file,
                                              std::string key,
                                              AVBooleanResultCallback resultCallback);

  std::string generateQiniuMultipartBodyString(std::string bucket,
                                               std::string token,
                                               AVFile* file,
                                               std::string key);

public:
  static UploaderManager* sharedInstance();
  static std::string generateRandomString(int length);
  static std::string generateQiniuKey();
  void uploadWithAVFileAndCallback(AVFile* file,
                                   AVBooleanResultCallback resultCallback);
  void release();
};

NS_AV_END

#endif  // INCLUDE_AVFILE_UPLOADERMANAGER_H_
