/**
 * @file   AVFile.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Aug 19 17:50:53 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "AVFile/AVFile.h"

#include <string>
#include <exception>
#include <boost/log/trivial.hpp>
#include "AVFile/UploaderManager.h"
#include "Request/AVPaasClient.h"

NS_AV_BEGIN

const std::string ownerTag = "owner";
const std::string fileSizeTag = "size";
const std::string fileMd5Tag = "_checksum";

AVFile* AVFile::fileWithPath(std::string path) {
  AVFile* file = new AVFile();
  file->path = path;
  file->name = StringUtils::getFileNameFromPath(path);

  return file;
}

AVFile* AVFile::fileWithUrl(std::string url) {
  AVFile* file = new AVFile();
  file->url = url;
  file->name = StringUtils::getFileNameFromPath(url);

  return file;
}

AVFile* AVFile::fileWithObjectId(std::string objectId) {
  AVFile* file = new AVFile();
  file->objectId = objectId;

  return file;
}

void AVFile::release() {
  AV_SAFE_DELETE(this);
}

Json::Value AVFile::updateMetadata() {
  if (!this->metadata.isMember(ownerTag)) {
    // Objective-C code is here
    // NSString * objectId = [AVPaasClient sharedInstance].currentUser.objectId;
    // if (objectId.length > 0) {
    //     [self.metadata setObject:objectId forKey:ownerTag];
    // }
  }

  if (!this->metadata.isMember(fileSizeTag)) {
    std::ifstream is(this->path, std::ifstream::binary);
    if (is) {
      is.seekg(0, is.end);
      int length = is.tellg();
      is.seekg(0, is.beg);
      is.close();

      if (length > 0) {
        this->metadata[fileSizeTag] = length;
      }
    }
  }

  if (!this->metadata.isMember(fileMd5Tag)) {
    if (this->path.length() > 0) {
      std::string md5 = StringUtils::AVMD5String(this->path);
      if (md5.length() > 0) {
        this->metadata[fileMd5Tag] = md5;
      }
    }
  }

  return this->metadata;
}

void AVFile::saveInBackground() {
  AVBooleanResultCallback callback;
  this->saveInBackgroundWithCallback(callback);
}

void AVFile::saveInBackgroundWithCallback(AVBooleanResultCallback callback) {
  UploaderManager::sharedInstance()->
      uploadWithAVFileAndCallback(this,
                                  [&](bool const& succeeded, AVError const& error) {
                                    if (!succeeded) {
                                      this->deleteInBackground();
                                    }

                                    callback(succeeded, error);
                                  });
}

void AVFile::fetchFileDataIntoPath(std::string path) {
  if (this->url.length() > 0) {
    AVPaasClient::sharedInstance()->fetchFileDataIntoPathWithUrl(path,
                                                                 this->url);
  }
}

void AVFile::deleteInBackground() {
  if (this->objectId.length() > 0) {
    Json::Value parameters;

    AVPaasClient::sharedInstance()->
        deleteObject(AVFile::objectPath(this->objectId),
                     parameters,
                     [&](Json::Value const & root, AVError const & error) {
                     });
  }
}

///////////////////////////////private methods /////////////////////////////
std::string AVFile::objectPath(std::string objectId) {
  if (objectId.length() > 0) {
    return StringUtils::string_format("classes/_File/%s", objectId);
  }

  return "classes/_File";
}


NS_AV_END
