/**
 * @file   AVRelation.cpp
 * @author yang chaozhong <cyang@avoscloud.com>
 * @date   Thu Aug 28 16:54:51 2014
 *
 * @brief
 *
 * Copyright 2014 AVOS Cloud Inc. All rights reserved.
 */

#include "AVObject/AVRelation.h"

#include <string>

NS_AV_BEGIN

AVQuery* AVRelation::query() {
  Json::Value dict(Json::objectValue);
  Json::Value relatedToDict(Json::objectValue);
  Json::Value objectDict(Json::objectValue);
  objectDict["__type"] = "Pointer";
  objectDict["className"] = this->parent->className;
  if (this->parent->hasValidObjectId()) {
    objectDict["objectId"] = this->parent->objectId;
  }

  relatedToDict["object"] = objectDict;
  relatedToDict["key"] = this->key;
  dict["$relatedTo"] = relatedToDict;

  AVQuery* query = AVQuery::queryWithClassName(this->targetClass);
  query->setWhere(dict);

  return query;
}

void AVRelation::release() {
  AV_SAFE_DELETE(this);
}

void AVRelation::addObject(AVObject* object) {
  if (object->hasValidObjectId()) {
    this->targetClass = object->className;
    this->parent->addRelationForKey(object, this->key);
  }
}

void AVRelation::removeObject(AVObject* object) {
  this->parent->removeRelationForKey(object, this->key);
}

AVQuery* AVRelation::reverseQueryWithRelationKeyAndChildObject(std::string parentClassName,
                                                               std::string relationKey,
                                                               AVObject* child) {
  Json::Value objectDict(Json::objectValue);
  objectDict["__type"] = "Pointer";
  objectDict["className"] = child->className;
  if (child->hasValidObjectId()) {
    objectDict["objectId"] = child->objectId;
  }

  AVQuery* query = AVQuery::queryWithClassName(parentClassName);
  query->setWhere(objectDict);

  return query;
}

NS_AV_END
