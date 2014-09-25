/**
 * @file   AVFile.h
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Fri Jul 18 17:48:38 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#ifndef INCLUDE_AVFILE_AVFILE_H_
#define INCLUDE_AVFILE_AVFILE_H_

#include <string>
#include <fstream>
#include "AVConstants.h"
#include "Utils/AVPlatformMacros.h"

NS_AV_BEGIN

class AVFile {
private:
  AVFile() {}
  static std::string objectPath(std::string objectId);

public:
  /**
   * The name of the file
   *
   */
  std::string name;

  /**
   * The local path of the file
   *
   */
  std::string path;

  /**
   * The Qiniu bucket of the file.
   *
   */
  std::string bucket;

  /**
   * The id of the file.
   *
   */
  std::string objectId;

  /**
   * The url of the file.
   *
   */
  std::string url;

  /**
   * The metadata of the file.
   *
   */
  Json::Value metadata;

public:
  /**
   * Creates a file with the local path of the file.
   *
   * @param path The path of the file that will upload to AVOS Cloud.
   *
   * @return an AVFile
   */
  static AVFile* fileWithPath(std::string path);

  /**
   * Creates a file with given url.
   *
   * @param url The url of file.
   *
   * @return an AVFile
   */
  static AVFile* fileWithUrl(std::string url);

  /**
   * Creates a file with given url.
   *
   * @param objectId The objectId of file.
   *
   * @return an AVFile
   */
  static AVFile* fileWithObjectId(std::string objectId);

  /**
   * release memory.
   *
   */
  void release();

  /**
   * Update metadata of the file.
   *
   * @return
   */
  Json::Value updateMetadata();

  /**
   * Saves the file.
   *
   */
  void saveInBackground();

  /**
   * Saves the file and executes the given callback.
   *
   * @param callback The callback should have the following argument signature: (bool const&, AVError const&)
   */
  void saveInBackgroundWithCallback(AVBooleanResultCallback callback);

  /**
   * fetch file data from server, and save it into local path.
   *
   * @param path The local path of file.
   */
  void fetchFileDataIntoPath(std::string path);

  /**
   * Delete the file.
   *
   */
  void deleteInBackground();
};

NS_AV_END

#endif  // INCLUDE_AVFILE_AVFILE_H_
