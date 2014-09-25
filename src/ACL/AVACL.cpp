/**
 * @file   AVACL.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Sep  2 14:31:47 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "ACL/AVACL.h"

#include <string>
#include "ACL/AVRole.h"
#include "User/AVUser.h"
#include "Request/AVPaasClient.h"
#include "Utils/AVStringUtils.h"

NS_AV_BEGIN

static std::string readTag = "read";
static std::string writeTag = "write";
static std::string publicTag = "*";

AVACL::AVACL() {
  if (AVPaasClient::sharedInstance()->defaultACL != nullptr) {
    this->permissionById = AVPaasClient::sharedInstance()->
                           defaultACL->permissionById;
  } else {
    this->permissionById = Json::Value(Json::objectValue);
  }
}

AVACL* AVACL::ACL() {
  AVACL* result = new AVACL();
  return result;
}

AVACL* AVACL::ACLWithUser(AVUser* user) {
  AVACL* result = new AVACL();
  result->setReadAccessForUser(true, user);
  result->setWriteAccessForUser(true, user);
  return result;
}

void AVACL::release() {
  AV_SAFE_DELETE(this);
}

Json::Value AVACL::dictionaryToReadAndWrite(bool read, bool write) {
  Json::Value dictionary(Json::objectValue);
  dictionary[readTag] = read;
  dictionary[writeTag] = write;
  return dictionary;
}

Json::Value AVACL::dictionaryForKeyIfCreate(std::string key, bool create) {
  Json::Value data(Json::nullValue);

  if (this->permissionById.isMember(key)) {
    data = this->permissionById[key];
  }

  if (data.isNull() && create) {
    data = Json::Value(Json::objectValue);
    this->permissionById[key] = data;
  }

  return data;
}

void AVACL::allowReadForKey(bool allowed, std::string key) {
  Json::Value data = this->dictionaryForKeyIfCreate(key, allowed);
  if (allowed) {
    data[readTag] = true;
  } else {
    data.removeMember(readTag);
  }

  this->permissionById[key] = data;
}

bool AVACL::isReadAllowedForKey(std::string key) {
  Json::Value data = this->dictionaryForKeyIfCreate(key, false);
  return data[readTag] == true;
}

void AVACL::allowWriteForyKey(bool allowed, std::string key) {
  Json::Value data = this->dictionaryForKeyIfCreate(key, allowed);
  if (allowed) {
    data[writeTag] = true;
  } else {
    data.removeMember(writeTag);
  }

  this->permissionById[key] = data;
}

bool AVACL::isWriteAllowedForyKey(std::string key) {
  Json::Value data = this->dictionaryForKeyIfCreate(key, false);
  return data[writeTag] == 1;
}

void AVACL::setPublicReadAccess(bool allowed) {
  this->allowReadForKey(allowed, publicTag);
}

bool AVACL::getPublicReadAccess() {
  return this->isReadAllowedForKey(publicTag);
}

void AVACL::setPublicWriteAccess(bool allowed) {
  this->allowWriteForyKey(allowed, publicTag);
}

bool AVACL::getPublicWriteAccess() {
  return this->isWriteAllowedForyKey(publicTag);
}

void AVACL::setReadAccessForUserId(bool allowed, std::string userId) {
  this->allowReadForKey(allowed, userId);
}

bool AVACL::getReadAccessForUserId(std::string userId) {
  return this->isReadAllowedForKey(userId);
}

void AVACL::setWriteAccessForUserId(bool allowed, std::string userId) {
  this->allowWriteForyKey(allowed, userId);
}

bool AVACL::getWriteAccessForUserId(std::string userId) {
  return this->isWriteAllowedForyKey(userId);
}

void AVACL::setReadAccessForUser(bool allowed, AVUser* user) {
  if (user->hasValidObjectId()) {
    this->allowReadForKey(allowed, user->objectId);
  }
}

bool AVACL::getReadAccessForUser(AVUser* user) {
  if (user->hasValidObjectId()) {
    return this->isReadAllowedForKey(user->objectId);
  } else {
    // Default value is true
    return true;
  }
}

void AVACL::setWriteAccessForUser(bool allowed, AVUser* user) {
  if (user->hasValidObjectId()) {
    this->allowWriteForyKey(allowed, user->objectId);
  }
}

bool AVACL::getWriteAccessForUser(AVUser* user) {
  if (user->hasValidObjectId()) {
    return this->isWriteAllowedForyKey(user->objectId);
  } else {
    // Default value is true
    return true;
  }
}

std::string AVACL::roleName(std::string name) {
  return StringUtils::string_format("role:%s", name);
}


void AVACL::setReadAccessForRoleWithName(bool allowed, std::string name) {
  this->allowReadForKey(allowed, this->roleName(name));
}

bool AVACL::getReadAccessForRoleWithName(std::string name) {
  return this->isReadAllowedForKey(this->roleName(name));
}

void AVACL::setWriteAccessForRoleWithName(bool allowed, std::string name) {
  this->allowWriteForyKey(allowed, this->roleName(name));
}

bool AVACL::getWriteAccessForRoleWithName(std::string name) {
  return this->isWriteAllowedForyKey(this->roleName(name));
}

void AVACL::setReadAccessForRole(bool allowed, AVRole* role) {
  this->allowReadForKey(allowed, this->roleName(role->name));
}

bool AVACL::getReadAccessForRole(AVRole* role) {
  return this->isReadAllowedForKey(this->roleName(role->name));
}

void AVACL::setWriteAccessForRole(bool allowed, AVRole* role) {
  this->allowWriteForyKey(allowed, this->roleName(role->name));
}

bool AVACL::getWriteAccessForRole(AVRole* role) {
  return isWriteAllowedForyKey(this->roleName(role->name));
}

void AVACL::setDefaultACLWithCurrentUser(AVACL* acl, bool currentUserAccess) {
  AVPaasClient::sharedInstance()->defaultACL = acl;
  AVPaasClient::sharedInstance()->
      currentUserAccessForDefaultACL = currentUserAccess;
}

NS_AV_END
