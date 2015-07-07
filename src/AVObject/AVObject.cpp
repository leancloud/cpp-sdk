/**
 * @file   AVObject.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Tue Jul 29 11:20:25 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "AVObject/AVObject.h"

#include <string>
#include <vector>
#include <algorithm>

#include "AVObject/AVRelation.h"
#include "Request/AVPaasClient.h"

NS_AV_BEGIN

using boost::any_cast;

AVObject::AVObject():objectId(),
	             addedRelationData(),
                     removedRelationData() {
}

AVObject* AVObject::objectWithClassName(std::string const & className) {
  AVObject *object = new AVObject();
  object->className = className;

  return object;
}

AVObject* AVObject::objectWithoutDataWithClassName(std::string const & className,
                                                   std::string const & objectId) {
  AVObject *object = new AVObject();
  object->className = className;
  object->objectId = objectId;

  return object;
}

void AVObject::release() {
  AV_SAFE_DELETE(this);
}

void AVObject::setACL(AVACL* acl) {
  this->acl = acl;
  this->localData["ACL"] = acl->permissionById;
}

AVACL* AVObject::getACL() {
  return this->acl;
}

std::vector<std::string> AVObject::allKeys() {
  std::vector<std::string> keys;
  for (auto it = this->localData.begin(); it != this->localData.end(); ++it) {
    keys.push_back(it.key().asString());
  }

  return keys;
}

Json::Value AVObject::objectForKey(std::string const & key) {
  return this->localData[key];
}

void AVObject::setObjectForKey(Json::Value const & object,
                               std::string const & key) {
  this->localData[key] = object;
}

void AVObject::setObjectForKey(AVObject* const & object,
                               std::string const & key) {
  if (object->hasValidObjectId()) {
    Json::Value value(Json::objectValue);
    value["__type"] = "Pointer";
    value["className"] = object->className;
    value["objectId"] = object->objectId;

    this->localData[key] = value;
  }
}

void AVObject::setObjectForKey(AVGeoPoint* const & geoPoint,
                               std::string const & key) {
  Json::Value value = AVGeoPoint::dictionaryFromGeoPoint(geoPoint);
  this->localData[key] = value;
}

void AVObject::removeObjectForKey(std::string const & key) {
  this->localData.removeMember(key);
}

AVRelation* AVObject::relationForKey(std::string key) {
  std::vector<AVObject*> array;
  if (this->addedRelationData.find(key) != this->addedRelationData.end()) {
    array = this->addedRelationData[key];
  } else {
    this->addedRelationData[key] = array;
  }

  AVObject* target = nullptr;
  if (array.size() > 0) {
    target = array[0];
  }

  AVRelation* relation = new AVRelation();
  relation->parent = this;
  relation->key = key;

  if (target != nullptr) {
    relation->targetClass = target->className;
  }

  return relation;
}

void AVObject::addRelationForKey(AVObject* object, std::string key) {
  this->addObjectIntoRelationDataForKey(&(this->addedRelationData), object, key);
}

void AVObject::removeRelationForKey(AVObject* object, std::string key) {
  this->addObjectIntoRelationDataForKey(&(this->removedRelationData), object, key);
}

void AVObject::addObjectIntoRelationDataForKey(std::unordered_map<std::string, std::vector<AVObject*>> *relationData,
                                               AVObject* object,
                                               std::string key) {
  if (object->hasValidObjectId()) {
    auto got = relationData->find(key);
    if (got == relationData->end()) {
      std::vector<AVObject*> array;
      array.push_back(object);
      (*relationData)[key] = array;
    } else {
      got->second.push_back(object);
    }
  }
}

Json::Value AVObject::generateRelationObjectsByArray(std::vector<AVObject*> objects,
                                                     bool isAdded) {
  Json::Value root;
  if (isAdded) {
    root["__op"] = "AddRelation";
  } else {
    root["__op"] = "RemoveRelation";
  }

  root["objects"] = Json::arrayValue;
  for (auto& object : objects) {
    Json::Value obj(Json::objectValue);
    obj["__type"] = "Pointer";
    obj["className"] = object->className;
    obj["objectId"] = object->objectId;
    root["objects"].append(obj);
  }

  return root;
}

void AVObject::saveInBackground() {
  this->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      // do nothing
    });
}

void AVObject::saveInBackgroundWithCallback(AVBooleanResultCallback callback) {
  for (auto& kv : this->addedRelationData) {
    std::string key = kv.first;
    std::vector<AVObject*> objects = kv.second;

    this->localData[key] = this->generateRelationObjectsByArray(objects, true);
  }

  for (auto& kv : this->removedRelationData) {
    std::string key = kv.first;
    std::vector<AVObject*> objects = kv.second;

    this->localData[key] = this->generateRelationObjectsByArray(objects, false);
  }

  this->addedRelationData.clear();
  this->removedRelationData.clear();
  if (this->hasValidObjectId()) {
    std::string sessionToken;
    AVPaasClient::sharedInstance()->
        putObject(this->myObjectPath(),
                  this->localData,
                  sessionToken, [&](Json::Value const & root,
                                    AVError const & error){
                    if (error.domain.length() == 0) {
                      callback(true, error);
                    } else {
                      callback(false, error);
                    }
                  });
  } else {
    AVPaasClient::sharedInstance()->
        postObject(this->myObjectPath(),
                   this->localData,
                   [&](Json::Value const & root,
                       AVError const & error){
                     if (error.domain.length() == 0) {
                       this->objectId = root["objectId"].asString();
                       this->createdAt = root["createdAt"].asString();

                       callback(true, error);
                     } else {
                       callback(false, error);
                     }
                   });
  }
}

void AVObject::saveAllInBackground(std::vector<AVObject*> objects) {
  for (auto & object : objects) {
    object->saveInBackground();
  }
}

void AVObject::saveAllInBackgroundWithCallback(std::vector<AVObject*> objects,
                                               AVBooleanResultCallback callback) {
  for (auto & object : objects) {
    object->saveInBackgroundWithCallback(callback);
  }
}

void AVObject::fetch() {
  std::vector<std::string> keys;
  this->fetchWithKeys(keys);
}

void AVObject::fetchWithKeys(std::vector<std::string> keys) {
  if (this->hasValidObjectId()) {
    Json::Value parameters;

    if (keys.size() > 0) {
      std::string includeKeys;
      for (auto& key:keys) {
        includeKeys.append(key);
        includeKeys.append(",");
      }

      parameters["include"] = includeKeys;
    }

    AVPaasClient::sharedInstance()->
        getObject(this->myObjectPath(),
                  parameters,
                  [&](Json::Value const & root,
                      AVError const & error){
                    if (error.domain.length() == 0) {
                      for (auto it = root.begin(); it != root.end(); ++it) {
                        std::string key = it.key().asString();

                        if (key == "createdAt") {
                          std::string value = (*it).asString();
                          this->createdAt = value;
                        } else if (key == "updateAt") {
                          std::string value = (*it).asString();
                          this->updatedAt = value;
                        }
                      }

                      this->localData = root;
                      this->localData.removeMember("objectId");
                      this->localData.removeMember("createdAt");
                      this->localData.removeMember("updatedAt");
                    }
                  });
  }
}

void AVObject::deleteInBackground() {
  this->deleteInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      // do nothing
    });
}

void AVObject::deleteInBackgroundWithCallback(AVBooleanResultCallback callback) {
  if (this->hasValidObjectId()) {
    Json::Value parameters;
    AVPaasClient::sharedInstance()->
        deleteObject(this->myObjectPath(),
                     parameters,
                     [&](Json::Value const & root,
                         AVError const & error){
                       if (error.domain.length() == 0) {
                         callback(true, error);
                       } else {
                         callback(false, error);
                       }
                     });
  }
}

bool AVObject::hasValidObjectId() {
  return this->objectId.length() > 0;
}

///////////////////////////////private methods /////////////////////////////
std::string AVObject::myObjectPath() {
  if (this->hasValidObjectId()) {
    return StringUtils::string_format("classes/%s/%s", this->className, this->objectId);
  } else {
    return StringUtils::string_format("classes/%s", this->className);
  }
}

NS_AV_END
